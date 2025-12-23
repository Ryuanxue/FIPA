CLIENT = nginx_sslkey_auth_idl_client
SERVER = nginx_sslkey_auth_idl_server

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
SOURCES.x = nginx_sslkey_auth_idl.x

TARGETS_SVC.c = nginx_sslkey_auth_idl_svc.c nginx_sslkey_auth_idl_server.c nginx_sslkey_auth_idl_xdr.c 
TARGETS_CLNT.c = nginx_sslkey_auth_idl_clnt.c nginx_sslkey_auth_idl_client.c nginx_sslkey_auth_idl_xdr.c 
TARGETS = nginx_sslkey_auth_idl.h nginx_sslkey_auth_idl_xdr.c nginx_sslkey_auth_idl_clnt.c nginx_sslkey_auth_idl_svc.c nginx_sslkey_auth_idl_client.c nginx_sslkey_auth_idl_server.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)
# Compiler flags 

CFLAGS += -g 
LDLIBS += -lnsl
RPCGENFLAGS = 

# Targets 

all : $(CLIENT) $(SERVER)

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

 clean:
	 $(RM) core $(TARGETS) $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER)