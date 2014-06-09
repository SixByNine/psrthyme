#!/bin/bash


BOOST=/Users/mkeith/install/boost_1_55_0/

wd=`dirname $0`

cd $wd/src/
$BOOST/dist/bin/bcp --boost=$BOOST --scan */*.cpp boost/


