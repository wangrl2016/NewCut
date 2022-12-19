import platform
import subprocess
import sys


def init_project():
    if len(sys.argv) < 2:
        print('Error: invalid argument')
        print('Usage: ' + sys.argv[0] + ' build_dir')
        exit(0)

    out_dir = sys.argv[1]

    print('Computer version: ' + platform.platform())

    # Check conan dependence
    try:
        subprocess.check_call(['conan', '--version'])
    except(OSError,):
        print('Dependence error: conan not install')
        exit(0)

    # Check git dependence
    try:
        subprocess.check_call(['git', '--version'])
    except(OSError,):
        print('Dependence error: git not install')
        exit(0)

    print('Dependencies is OK')


if __name__ == '__main__':
    init_project()
