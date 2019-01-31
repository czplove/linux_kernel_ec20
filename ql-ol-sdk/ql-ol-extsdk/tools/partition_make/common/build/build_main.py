#@PydevCodeAnalysisIgnore
'''build_main.py:

Usage:

      build_main [<build>:<directory> ...] [<var>=<value> ...]

   Examples:

      build_main modem:M8960AAAAANAAM0045 apps:M8960AAAAANLGA100001 modem_bid=AAAAANAA

      build_main modem:e:\\work\\8960_e\\0045_mb_a apps:M8960AAAAANLGA100001 modem_bid=AAAAANAA

Description:

This script is used to generate a Metabuild based on input from the command line.
It takes a series of name/value pairs that be of either of two formats:

   build:location

      build can be any of the support build types.  For 8960, these are 'apps', 'modem',
      'dsps', 'boot', 'lpass', 'rpm' and 'wcnss'.

      location can be either a directory or a build ID.  If the location is a build ID,
      build_main.py will call 'findbuild' to find the location of the build.

   var=value

      var represents a variable in the contents.xml file, i.e. something of the format
      ${var}.

      value is the new value to assign to that variable.  The resulting format will be
      ${var:value}

build_main.py will perform the following tasks:

   - Validate contents template.
   
   - Update contents.xml with data from the command line.

   - Generate "goto" scripts for each of the named builds.

   - Copy binaries (pointed to by contents.xml) into a fat.bin file.

   - Process partition.xml into rawprogram.xml and patch.xml

The tools to do these last two tasks are found through pointers in the contents.xml file.
-------------------------------------------------------------------------------

  $Header: //components/rel/build.glue/1.0/build_main.py#14 $
  $DateTime: 2017/02/27 17:16:08 $

===============================================================================
'''

import sys
import os
import stat
import subprocess
import shutil
import re
from glob import glob

print "build_main.py: Executing from", os.getcwd()

import lib.meta_log as lg 
import lib.meta_lib as ml
import lib.update_meta as um
import lib.validate_contents_template as vct
from xml.etree import ElementTree as et

#---------------------------------------------------------#
# Find our parameters                                     #
#---------------------------------------------------------#
lg=lg.Logger('build_main')
lg.log("build_main.py:Finding Parameters")
param_file_fn = 'latest_args.txt'
params = sys.argv[1:]

#Flag to check if script needs to be exited after contents template validation
exit_after_vct = False
if len(params) > 0:
   # For script help: consider if '-help' is 1st argument 
   if params[0].startswith('-help'):
      print __doc__
      sys.exit()
	  
   # Atleast 1 new argument is provided
   # Ignore latest_args.txt and overwrite
   param_file = open(param_file_fn, 'w')
   for p in params:
      param_file.write (p + '\n')
   param_file.close()
elif os.path.exists(param_file_fn):
   # No new args - Read from latest_args.txt
   param_file = open(param_file_fn, 'r')
   params = param_file.readlines()
   param_file.close()
else:
   #Set flag to exit script after validating contents template
   exit_after_vct = True

#-------------------------------------------------------#
# Validating all contents template avialable in glue
#-------------------------------------------------------#
lg.log_highlight("BUILD_MAIN.PY - START VALIDATING CONTENTS TEMPLATE")
contents_template_list = glob('../config/contents_template*.xml')
#True in case there is no default contents template
contents_validation_failed = len(contents_template_list) <= 0 
if contents_validation_failed:
   lg.log("Error: Default contents template is missing!!!")
   sys.exit()
# Found contents template
for content_template in contents_template_list:
   #Start contents validation
   content_template = os.path.abspath(content_template)
   invalid_xml_tag = vct.validate_contents_template(content_template)
   lg.log("Contents Template : " + content_template)
   if( len(invalid_xml_tag) > 0 ):
      lg.log('Error: Contents template validation failed! Please verify below unsupported XML tags\n'+'\n'.join( invalid_xml_tag ))
      contents_validation_failed = True 
   else:
      #Validation passed
      lg.log("***Validation PASSED***")

# Abort packaging in case of contents template validation failed 
if contents_validation_failed:
   lg.log("Error: Abort packaging")
   sys.exit()

# User only want to validate contents template (Ex:glue image CRM build)
if exit_after_vct:
   #Create the dummy file "../../build_success.txt" and write something into it
   #this is to indicate to CRM tools that build is successful
   lg.log("Generating build_success.txt")
   file_handle = file('../../build_success.txt','w')
   file_handle.write('./build_success.txt')
   file_handle.close()
   #Create 'BuildProducts.txt' file , which will enlist the files/artifacts to check by the CRM. 
   lg.log("Generating BuildProducts.txt")
   file_handle = file('../../BuildProducts.txt','w')
   file_handle.write('./build_success.txt')
   file_handle.close()
   lg.log("build_main.py:Done")
   sys.exit()

# Contents template validation passed, procced with meta build packing
ignore_missing_files = False
gen_minimized        = False
fb_nearest           = False
variant              = ''
wflow_filters        = ''

for p in params[:]:  # [:] syntax makes a copy of the list so we can modify it in the for loop.
   if p.startswith('-imf'): 
      ignore_missing_files = True
      params.remove(p)
   elif p.startswith('-minimized'):
     gen_minimized = True
     params.remove(p)
   elif p.startswith('-fb_nearest'):
     fb_nearest = True
     params.remove(p)
   elif p.startswith('variant='):
     variant =  p.strip().split('=')[-1]
   elif p.startswith('-params'):
     config_name = p.strip().split('=')[-1]
     param_index = params.index(p)
     root_dir = os.path.join( os.path.dirname( __file__), '../../' )
     build_config_path = os.path.join( root_dir , 'build_cfg.xml')
     lg.log("build_config_path: " + build_config_path )
     if os.path.exists(build_config_path):
        # Initialize the XML tree
        lg.ReleaseXmlTree = et.ElementTree(file = build_config_path)
        ReleaseXmlRoot = lg.ReleaseXmlTree.getroot()
        for config in ReleaseXmlRoot.findall('build_params'):
            if config.get('name').upper() == config_name.upper():
                params.remove(p)
                for c in config.text.strip().split():
                    params.insert(param_index,c.strip())
                    param_index += 1

import datetime
#import socket
#lg.log('Current machine is: '   + socket.gethostname())
lg.log('Current directory is: ' + os.getcwd(), verbose=0)
lg.log('Current date/time is: ' + str(datetime.datetime.now())) 
lg.log('Variant: ' + str(variant)) 

# Log the OS environment
lg.log("OS Environment:")
for var in os.environ:
   lg.log("   " + var + ' = ' + os.environ[var])

# Add meta build ID to the XML file
meta_id = 'Unknown'
if 'CRM_BUILDID' in os.environ:
   meta_id = os.environ['CRM_BUILDID']
   lg.log("OS Environment CRM_BUILDID Found")
elif 'JobName' in os.environ:
   meta_id = os.environ['JobName'].split('_')[0]
else:
   on_linux = sys.platform.startswith("linux")
   findmetaid_regex = '[/\\\\]([^/\\\\]+)[/\\\\]common[/\\\\]build'
   m = re.search(findmetaid_regex, os.getcwd())
   if (m):
      meta_id = m.group(1).upper()
params.append("common:{0}".format(meta_id))

#---------------------------------------------------------#
# UPDATE META
#---------------------------------------------------------#
lg.log_highlight("BUILD_MAIN.PY - CALLING UPDATE_META.PY")  
meta_contents_xml          = '../../contents.xml'
meta_contents_template = '../config/contents_template.xml'
if len(variant) > 0:
   template_file = '../config/contents_template_' + variant + '.xml'
   if os.path.exists(template_file):
      meta_contents_template = template_file

lg.log("Selected Template file:  " + meta_contents_template)	  
if not os.path.exists(meta_contents_xml):
   shutil.copy(meta_contents_template, meta_contents_xml)
os.chmod(meta_contents_xml, stat.S_IREAD | stat.S_IWRITE | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH | stat.S_IWOTH)
ret_val = um.update_meta(lg_obj=lg,pairs = params,fb_nearest = fb_nearest)
if ret_val:
   sys.exit(ret_val)

#---------------------------------------------------------#
# Create a file to store the version info
#---------------------------------------------------------#

# Initialize meta info object
mi = ml.meta_info(logger=lg)


#---------------------------------------------------------#

# Create ver_info.txt file in common/build folder with common build id
bid_file = os.path.join(os.path.dirname(__file__), 'Ver_Info.txt')
lg.log("Creating the file verinfo.txt to hold the buildID information at " + bid_file)
lg.log("The file will be stored at /firmware/verinfo/Ver_Info.txt ")
file_handle = open(bid_file,'w')
BuildID = mi.get_build_id('common', 'builds_flat')
file_handle.write(BuildID)
file_handle.close()

# BUILD.PY
#---------------------------------------------------------#
lg.log_highlight("BUILD_MAIN.PY - CALLING BUILD.PY")

build_cmd = ['python', 'build.py','--wflow_filter',wflow_filters]
ret_val_build = lg.log_exec(build_cmd)
lg.log('return value of build.py: ' + str( ret_val_build ) )
if( ret_val_build != 0 ):
   lg.log('Failing the meta-build because of error returned from build.py!!!')
   sys.exit( ret_val_build )

#---------------------------------------------------------#
# Call function to validate file paths in contents.xml
#---------------------------------------------------------#
if not ignore_missing_files:
   mi = ml.meta_info(logger=lg)
   lg.log_highlight("BUILD_MAIN.PY - VALIDATING FILE PATHS")
   try:
      um.validate_filepaths( mi, lg )
   except ml.InvalidFilePathException, ex:
      lg.log("build_main.py:InvalidFilePathException exception!!!, invalid file paths found in contents.xml ,count of invalid file paths = " + str(len( ex.invalid_files_list )))
      for invalidfile in ex.invalid_files_list:
         lg.log("build_main.py:Invalid file-path: " + invalidfile)
      print "build_main.py:InvalidFilePathException occurred" # exit the program
      sys.exit(1)
   lg.log("build_main.py:============== CHECK COMPLETE ===============\n")
   
#   lg.log("build_main.py:Calling gen_build_manifest")
#   lg.log_exec(['python', 'gen_build_manifest.py'])

#---------------------------------------------------------#
# Generate FFU image for WP
#---------------------------------------------------------#
if os.path.exists("build_relwp.bat"):
   if (lg.log_exec(['build_relwp.bat']) != 0):
      raise Exception,'build_relwp.bat failed.'

#---------------------------------------------------------#
# Create notes
#---------------------------------------------------------#
for build_name in mi.get_build_list():
   is_on_linux = 0
   build_path = mi.get_build_path(build_name)
   notes_file = build_path+"*_notes.txt"
   notes_file_paths = glob(notes_file) 
   for notes_file_path in notes_file_paths:
      in_file = open(notes_file_path, 'r')
      for line in in_file.readlines():
         if line.find("Host Env") > -1:
            if line.find("Linux") >-1:
               is_on_linux = 1
      if is_on_linux == 1:
         lg.log(build_name+" is built on linux\n")
      in_file.close()

#---------------------------------------------------------#
# Generate minimized build if required
#---------------------------------------------------------#
if gen_minimized:
   lg.log_highlight("BUILD_MAIN.PY - GENERATING MINIMIZED BUILD")
   destn_folder_name= meta_id + '_minibuild'
   try:
      os.makedirs(os.path.join('./../../',destn_folder_name))
      destn_file_path=os.path.join('./../../',destn_folder_name)
      lg.log_exec(['python', 'gen_minimized_build.py',destn_file_path]) 
   except WindowsError,e:
      lg.log("minimized build is already exist, to proceed please delete the already existing folder")

#-----------------------------------------------------------------------------------------------------#
# MetaBuild POST-Verification Tools                                                                   #
# This section includes execution of scripts from other teams to run verification on the build        #
#-----------------------------------------------------------------------------------------------------#

#-----parseContentsXML.py------#
#Purpose: To verify the existence of binaries required for download   
#LABEL: VU_CORE_STORAGE_ZENO_UTILITIES.XX  
#Owner:  CoreBSP Tools 
if (os.path.exists('parseContentsXML.py')):
   lg.log_highlight("BUILD_MAIN.PY - CALLING PARSEContentsXML.PY")
   ret_val_parseContentsXML = lg.log_exec(['python', 'parseContentsXML.py','--contentsxml='+os.path.abspath('../../contents.xml')])
   lg.log('\nbuild_main.py: Return value of parseContentsXML.py: ' + str( ret_val_parseContentsXML ) +"\n" )


#as build is now done, create the dummy file "../../build_success.txt" and write something into it
#this is to indicate to CRM tools that build is successful
lg.log("Generating build_success.txt")
file_handle = file('../../build_success.txt','w')
file_handle.write('./build_success.txt')
file_handle.close()

#Create 'BuildProducts.txt' file , which will enlist the files/artifacts to check by the CRM. 
lg.log("Generating BuildProducts.txt")
file_handle = file('../../BuildProducts.txt','w')
file_handle.write('./build_success.txt')
file_handle.close()

lg.log("build_main.py:Done")

