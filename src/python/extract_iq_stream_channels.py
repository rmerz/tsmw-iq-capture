#!/usr/bin/env python

import argparse
import fnmatch, os
import subprocess as sp

def setup_args ():
    parser = argparse.ArgumentParser(description='Extract all IQ data from stream files.')
    parser.add_argument('directory_path', type=str, help='File to open.')
    args   = parser.parse_args()
    return args

def find_files (directory_path):
    matches = list ()
    for root, dirnames, filenames in os.walk(directory_path):
        for filename in fnmatch.filter(filenames, '*000*'):
            matches.append(os.path.join(root, filename))
    return matches

def run_extract_iq (file_path_list):
    for file_path in file_path_list:
        print ('Process: ' + file_path)
        p = sp.call (["src/cpp/bin/extract_iq.exe","-s","-b",file_path[:-8]])
        

def main (args):
    # Find all stream files
    stream_files_list = find_files (args.directory_path)
    # Run extract_iq on them
    run_extract_iq (stream_files_list)
    pass

if __name__ == '__main__':
    args = setup_args ()
    main (args)
