#!/bin/bash


num_civ_insts=`ps aux | grep qemu.*android | grep -v grep | wc -l`

exit $num_civ_insts

