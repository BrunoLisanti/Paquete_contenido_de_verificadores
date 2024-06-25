#!/bin/bash 
cp server ../../eprom
cp server -rf ../image/eprom
cd ../image
tar cvfz pchecker.tgz ´find .´ .
mv pchecker.tgz ../installer