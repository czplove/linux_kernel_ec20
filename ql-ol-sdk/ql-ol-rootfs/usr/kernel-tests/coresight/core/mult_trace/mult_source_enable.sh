#!/bin/sh

# Copyright (c) 2013, The Linux Foundation. All rights reserved.
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


echo "-----Coresight Multiple Test Source Enable Test Starting-----"
echo "-------------------------------------------------------------"
source "$(dirname $0)/../cs_common.sh"
etm_enable_all_cores
retval=$?
if [ $retval -eq 1 ]
then
        echo "NA: ETM not present for all cores"
else
        stm_enable
        retval=$?
        if [ $retval -eq 1 ]
        then
                echo "NA: STM not present"
        else
                etm_test_if_enabled
                retval=$?
                if [ $retval -eq 1 ]
                then
                        echo "FAIL: ETM is not enabled"
                else
                        stm_test_if_enabled
                        retval=$?
                        if [ $retval -eq 1 ]
                        then
                                echo "FAIL: STM is not enabled"
                        else
                                echo "PASS: both ETM and STM are enabled"
                        fi
                fi
        fi
fi
echo "-----Coresight Multiple Test Source Enable Test Complete-----"
echo "-------------------------------------------------------------"
echo ""
