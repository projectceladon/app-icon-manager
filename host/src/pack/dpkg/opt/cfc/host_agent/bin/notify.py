#!/usr/bin/env python3

import sys, os, configparser, argparse, locale

def parseConfig(config_file):
    config = configparser.RawConfigParser()
    config.optionxform = str
    config_fp = open(config_file)
    config.read_file(config_fp)
    cfg = dict(config.items("Desktop Entry"))
    config_fp.close()
    return cfg

parser = argparse.ArgumentParser()
parser.add_argument('-P', '--package', help='Package Name', required=True)
parser.add_argument('-k', '--key', help='key', required=True)
parser.add_argument('-g', '--groupkey', help='groupkey', required=True)
parser.add_argument('-m', '--message', help='message', required=True)
parser.add_argument('-p', '--priority', help='priority', required=True)
args = parser.parse_args()

desktopfile = os.path.expanduser("~/applications/" + args.package + ".desktop")
cfg = {} 
if os.path.exists(desktopfile):
    cfg = parseConfig(desktopfile)
name = "Name[" + locale.getlocale()[0] + "]"
appname = args.package 
if name in cfg:
    appname = cfg[name]
else:
    print("Failed to get locale name, falling back to default")
    if "Name" in cfg:
        appname = cfg["Name"]

icon = ""
if "Icon" in cfg:
    icon = cfg["Icon"]

os.system("notify-send -i {} -a {} -c email '{}'".format(icon, appname, args.message))    
