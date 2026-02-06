import os
import sys

# NOTE: DOES NOT WORK WITH CURRENT DIRECTORY LAYOUT!!!

def main():
    i = 0
    app_dir = os.path.abspath('HAL/User-mode/Applications')
    f = open('ConfigGen/SampleCMakeLists.txt', 'r')
    sample = f.read()
    f.close()

    for dirname in os.listdir(app_dir):
        curr_app = os.path.join(app_dir, dirname)
        if os.path.isdir(curr_app):
            print(dirname)
            cmake_lists = sample.replace('@@AppName@@', dirname)
            cmake_lists = cmake_lists.replace('@@APP_NAME@@', dirname.upper())
            f = open(os.path.join(curr_app, 'CMakeLists.txt'), 'w')
            f.truncate()
            f.write(cmake_lists)
            f.close()
            i += 1
    print(f'Total: {i}')


if __name__ == '__main__':
    main()