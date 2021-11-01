#!/bin/bash

dpkg --list | grep "ii *android-virtual\|ii *vm-manager\|ii *cfc" | grep -v grep| wc -l

