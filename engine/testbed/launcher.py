#!/usr/bin/env python3

from ctypes import *

def main():
    testbed = cdll.LoadLibrary("./testbed.dylib")
    func = getattr(testbed, "main")

    ret = func()
    print("returned: "+ str(ret))
    return ret

if __name__ == '__main__':
    main()
