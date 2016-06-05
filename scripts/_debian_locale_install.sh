#!/bin/bash
OSPL="$(uname -s)"
ID_UTILITY="id"
if [ $OSPL = "SunOS" ]
then
	ID_UTILITY="/usr/xpg4/bin/id"
	OS=solaris
fi
if [ ! `$ID_UTILITY -u` = "0" ]
then
  echo "Please enter password of root:"
  if [ $OSPL = "SunOS" ]
  then
     echo "Solaris: Run as root !"
  elif [ $OSPL = "Linux" ]
  then
    if [ `grep -i debian /etc/issue | wc -l` -gt "0" ]
    then
      su --preserve-environment -c "bash $0"
    elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
    then
      sudo bash sh $0
    else
      echo "Run as root !"
    fi  
  fi
  exit
fi
dpkg-reconfigure locales #ru_RU.UTF-8
aptitude install console-cyrillic
dpkg-reconfigure console-cyrillic
/etc/init.d/console-cyrillic start
read
