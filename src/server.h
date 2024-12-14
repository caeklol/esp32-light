#ifndef WEBSERVER_HANDLER_H
#define WEBSERVER_HANDLER_H

String getPage(String content);
void handleHome();
void handleToggle();
extern void setupServerEndpoints();

#endif
