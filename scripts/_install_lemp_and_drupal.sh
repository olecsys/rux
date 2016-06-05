#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

current_os=$1
current_user=$2
current_user_group=$3
current_machine="$(uname -m)"

drupal_name=drupal-7.23
drupal_db_name=my_drupal_db
drupal_db_user=olecsys
drupal_db_user_password=4058olecsys1986

if [ -z $current_user ]; then
	current_user="$(whoami)"
fi
if [ -z $current_user_group ]; then
	current_user_group="$(id -g --name)"
fi
if [ -z $current_os ]; then
	current_os="$(uname -s)"
fi
ID_UTILITY="id"
install_success="false"
if [ $current_os = "SunOS" ]
then
	ID_UTILITY="/usr/xpg4/bin/id"
fi
if [ ! `$ID_UTILITY -u` = "0" ]
then
	if [ $current_os = "SunOS" ]
	then
		echo "Solaris: Run as root !"
	elif [ $current_os = "Linux" ]
	then
		if [ `grep -i debian /etc/issue | wc -l` -gt "0" ]
		then
			current_os="Debian"
			su --preserve-environment -c "bash $0 $current_os $current_user $current_user_group"
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then
			current_os="Ubuntu"
			sudo bash $0 $current_os $current_user $current_user_group
		else
			echo "Run as root !"
		fi
	elif [ $current_os = "Darwin" ]
	then
		echo "Mac X: Run as root !"
	elif [ $current_os = "HP-UX" ]
	then
		echo "HP-UX: Run as root !"
	else
		echo "UnknownOS: Run as root !"
	fi
	exit
fi
packets_program=""
if [ $current_os = "Ubuntu" ]
then
	packets_program="apt-get"
elif [ $current_os = "Debian" ]
then	
	packets_program="apt-get"
fi
if [ $packets_program = "apt-get" ]
then
	apt-get update && install_success="true"
	if [ $install_success = "true" ]
	then	
		install_success="false"
		apt-get install mysql-server php5-mysql && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		mysql_install_db && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		/usr/bin/mysql_secure_installation && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		apt-get install nginx && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		service nginx start && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		apt-get install php5-fpm && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/;cgi\.fix_pathinfo=1/cgi\.fix_pathinfo=0/g' /etc/php5/fpm/php.ini && install_success="true"	
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/cgi\.fix_pathinfo=1/cgi\.fix_pathinfo=0/g' /etc/php5/fpm/php.ini && install_success="true"		
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"		
		sed -i -e 's/listen = 127\.0\.0\.1:9000/listen = \/var\/run\/php5-fpm\.sock/g' /etc/php5/fpm/pool.d/www.conf && install_success="true"			
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		service php5-fpm restart && install_success="true"
	fi
	cd /tmp
	if [ $install_success = "true" ]
	then	
		install_success="false"
		wget http://ftp.drupal.org/files/projects/$drupal_name.tar.gz && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		tar xzvf $drupal_name.tar.gz && install_success="true"
	fi	
	rm -rf $drupal_name.tar.gz
	cd "$old_dir"	
	if [ $install_success = "true" ]
	then	
		install_success="false"
		echo "CREATE DATABASE IF NOT EXISTS "$drupal_db_name";
		USE mysql;
		drop procedure if exists createUser;
		delimiter \$\$
		create procedure createUser(username varchar(50), pw varchar(50))
		begin
		IF (SELECT EXISTS(SELECT 1 FROM \`mysql\`.\`user\` WHERE \`user\` = username)) = 0 THEN
		begin
			set @sql = CONCAT('CREATE USER ', username, '@\'localhost\' IDENTIFIED BY \'', pw, '\'');
			prepare stmt from @sql;
			execute stmt;
			deallocate prepare stmt;
		end;
		END IF;
		end \$\$
		delimiter ;
		call createUser('"$drupal_db_user"', '"$drupal_db_user_password"');
		GRANT ALL PRIVILEGES ON "$drupal_db_name".* TO "$drupal_db_user"@localhost IDENTIFIED BY '"$drupal_db_user_password"';
		FLUSH PRIVILEGES;" > "/tmp/create_drupal.sql" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		mysql -u root -p < "/tmp/create_drupal.sql" && install_success="true"
	fi	
	rm "/tmp/create_drupal.sql"	
	if [ $install_success = "true" ]
	then	
		if [ ! -d /usr/share/nginx/html/drupal ]
		then
			install_success="false"
			mkdir /usr/share/nginx/html/drupal && install_success="true"
		fi		
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		cp -rf "/tmp/$drupal_name/"* /usr/share/nginx/html/drupal/ && install_success="true"
	fi
	rm -rf /tmp/$drupal_name
	if [ $install_success = "true" ]
	then	
		install_success="false"
		cp /usr/share/nginx/html/drupal/sites/default/default.settings.php /usr/share/nginx/html/drupal/sites/default/settings.php && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		chmod a+w /usr/share/nginx/html/drupal/sites/default/settings.php && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		chmod a+w /usr/share/nginx/html/drupal/sites/default && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		apt-get install php5-gd && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		service php5-fpm restart && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		cp /etc/nginx/sites-available/default /etc/nginx/sites-available/drupal && install_success="true"
	fi	
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e '/\s*#.*/d' /etc/nginx/sites-available/drupal && install_success="true"		
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/\s*try_files \$uri \$uri\/ =404;/\t\ttry_files \$uri \$uri\/ \/index\.php?q=\$uri\&\$args;\n\t}\n\terror_page 404 \/404\.html;\n\terror_page 500 502 503 504 \/50x\.html;\n\tlocation = \/50x\.html {\n\t\troot \/usr\/share\/nginx\/html\/drupal;\n\t}\n\tlocation ~ \\\.php\$ {\n\t\tfastcgi_pass unix:\/var\/run\/php5-fpm\.sock;\n\t\tfastcgi_index index\.php;\n\t\tinclude fastcgi_params;/g' /etc/nginx/sites-available/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/\s*listen 80 default_server;/\tlisten 80;/g' /etc/nginx/sites-available/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e '/\s*listen \[::\]:80 default_server ipv6only=on;/d' /etc/nginx/sites-available/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/\s*root \/usr\/share\/nginx\/html;/\troot \/usr\/share\/nginx\/html\/drupal;/g' /etc/nginx/sites-available/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/\s*index index\.html index\.htm;/\tindex index\.php index\.html index\.htm;/g' /etc/nginx/sites-available/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		sed -i -e 's/\s*server_name localhost;/\tserver_name 127.0.0.1;/g' /etc/nginx/sites-available/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		ln -fs /etc/nginx/sites-available/drupal /etc/nginx/sites-enabled/drupal && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		rm -rf /etc/nginx/sites-enabled/default && install_success="true"
	fi
	if [ $install_success = "true" ]
	then	
		install_success="false"
		service nginx restart && install_success="true"
	fi
fi
red='\e[0;31m'
green='\e[0;32m'
nocolor='\e[0m'
if [ $install_success = "true" ]
then
	echo -e "${green}LEMP and Drupal installation succeeded.Press <ENTER> to continue${nocolor}"
else
	echo -e "${red}LEMP and Drupal installation failed.Press <ENTER> to continue${nocolor}"
fi
cd "$old_dir"
read $symbol
