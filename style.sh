#!/bin/sh
astyle --style=attach -n --indent=force-tab --indent-switches --indent-preproc-block --indent-preproc-define --pad-oper --pad-header --align-pointer=type -j *.c *.h
