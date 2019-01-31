#!/bin/sh
# Copyright (c) 2013, 2015 The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#     * Neither the name of The Linux Foundation nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
# ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# firmware-links        init.d script to install the firmware links
#

# No path is set up at this point so we have to do it here.
PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

# Check for images and set up symlinks
if [ -f /etc/init.d/fw ]
then
   chmod 777 /etc/init.d/fw
   swconfig=`cat /etc/init.d/fw`
   fwpath="/firmware/image/${swconfig}"
   if [ -d $fwpath -a "$(ls $fwpath)" ]
   then
      echo "fwpath exists"
   else
      fwpath="/firmware/image"
   fi
else
   fwpath="/firmware/image"
fi

echo $fwpath
cd $fwpath

# Get the list of files in /firmware/image
# for which sym links have to be created

fwfiles=$(ls modem* *.bin mba*)

# Check if the links with similar names
# have been created in /lib/firmware

mkdir -p /lib/firmware
cd /lib/firmware
linksNeeded=0

# For everyfile in fwfiles check if
# the corresponding file exists
for fwfile in $fwfiles
do
   # if (condition) does not seem to work
   # with the android shell. Therefore
   # make do with case statements instead.
   # if a file named $fwfile is present
   # no need to create links. If the file
   # with the name $fwfile is not present
   # need to create links.

   fw_file=$(ls $fwfile)
   if [ "$fw_file" == "$fwfile" ]
   then
      continue
   else
      linksNeeded=1
      break
   fi
done

case $linksNeeded in
   1)
      cd $fwpath

      case `ls modem.mdt 2>/dev/null` in
         modem.mdt)
            for imgfile in modem*
            do
               ln -s $fwpath/$imgfile /lib/firmware/$imgfile 2>/dev/null
            done
            ;;
        *)
            # trying to log here but nothing will be logged since it is
            # early in the boot process. Is there a way to log this message?
            echo "PIL no modem image found"
            ;;
      esac

      case `ls *.bin 2>/dev/null` in
         *.bin)
            for imgfile in *.bin
            do
               ln -s $fwpath/$imgfile /lib/firmware/$imgfile 2>/dev/null
            done
            ;;
         *)
            echo "PIL no wcnss image found"
            ;;
      esac

      case `ls *.tlv 2>/dev/null` in
         *.tlv)
            for imgfile in *.tlv
            do
               ln -s $fwpath/$imgfile /lib/firmware/$imgfile 2>/dev/null
            done
            ;;
         *)
            echo "PIL no wcnss image found"
            ;;
      esac

      case `ls mba.mdt 2>/dev/null` in
         mba.mdt)
            for imgfile in mba*
            do
               ln -s $fwpath/$imgfile /lib/firmware/$imgfile 2>/dev/null
            done
            ;;
         *)
            echo "PIL no mba image found"
            ;;
      esac
	  ;;

   *)
      echo "Nothing to do. No firmware links needed."
      ;;
esac

cd /
exit 0
