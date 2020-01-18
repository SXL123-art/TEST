#ifndef USBUPDATE_H
#define USBUPDATE_H

#define USBUPDATEPATH	"/mnt/release/update.tar.bz2"

int udisk(int i);
int updateproc(void);
int updateproc_tar(void);
int update_usb(void);
#endif
