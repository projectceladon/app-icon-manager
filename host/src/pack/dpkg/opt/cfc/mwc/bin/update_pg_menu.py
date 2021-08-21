#!/usr/bin/env python3

import sys

try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser

def update_menu_ini_file(icon_file, menu_ini_file):
    config = ConfigParser()
    config.read(menu_ini_file)
    app_section = config['application']
    if icon_file in app_section:
        int_val = config.getint('application', icon_file)
        print (icon_file + " with index: " + str(int_val) + " already added into menu ini file: " + menu_ini_file)
    else:
        # get current max index
        max_val = 0;
        for key in app_section:
            int_val = app_section.getint(key)
            if (int_val > max_val):
                max_val = int_val
        print ("Max app index: " + str(max_val))
        max_val = max_val + 1
        app_section[icon_file] = str(max_val);
        with open(menu_ini_file, 'w') as menu_config_file:
            config.write(menu_config_file)
            print ("App icon_file: " + icon_file + " with index: " + str(max_val) + " added into: " + menu_ini_file)

def main(argv):
    print ("Icon file: ", argv[0])
    print ("Menu ini file: ", argv[1])
    update_menu_ini_file(argv[0], argv[1])

if __name__ == "__main__":
    if (len(sys.argv) != 3):
        print (sys.argv[0]+" your_app.desktop your_menu.ini")
        sys.exit(1)
    main(sys.argv[1:])
