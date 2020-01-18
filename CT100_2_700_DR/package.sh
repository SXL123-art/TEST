#   > File Name: package.sh
#   > Author: zss
#   > Mail: 315261906@qq.com 
#   > Created Time: 2018年09月07日 星期五 15时15分37秒
 
# rm -rf ./update/
# mkdir update
# cd update
# mkdir ./Application/
# cd ../
 
 #执行全包 ./package.sh  任意参数
 #只打包应用程序 ./package.sh
 
 #--------$# != 0表示脚本带有传参，带有参数表示打包所有文件
 if [ $# != 0 ];then
 
	 #不是全包要注释
	 cp -rf ./backups/lib ./update/
	 
	 #不是全包要注释
	 #cp -rf ./backups/wifi_8188 ./update/

	 #不是全包要注释
	 cp -rf ./backups/vsftpd ./update/
	 

	  #不是全包要注释
	 cp -rf ./backups/Application/* ./update/Application/
 else
	 cp -rf ./backups/Application/break.sh ./update/Application/
	 cp -rf ./backups/Application/ct100.sh ./update/Application/
 fi
 
cp -rf ./backups/update.sh ./update/
#注意拷贝这里的路径
cp -rf ./build/CT100App/CT100App ./update/Application/
cp -rf ./build/CT100Man/CT100Man ./update/Application/
cp -rf ./build/CT100Qt/CT100Qt ./update/Application/
 
sleep 1s 
tar -jcvf update.tar.bz2 update
