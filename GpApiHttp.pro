TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=829f25d3-bc89-4f1c-a950-1dee504c8b38
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER
DEFINES		   += GPAPI_HTTP_LIBRARY
PACKET_NAME     = GpApiHttp
DIR_LEVEL       = ./..

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
	GP_CORE_LIB_V		= 2
	GP_API_LIB_V		= 0
	GP_JSON_LIB_V		= 0
	GP_NETWORK_LIB_V	= 0
}

os_linux{
}

LIBS += -lGpCore2$$TARGET_POSTFIX$$GP_CORE_LIB_V
LIBS += -lGpApi$$TARGET_POSTFIX$$GP_API_LIB_V
LIBS += -lGpJson$$TARGET_POSTFIX$$GP_JSON_LIB_V
LIBS += -lGpNetwork$$TARGET_POSTFIX$$GP_NETWORK_LIB_V

#------------------------------ LIBS END ---------------------------------

SOURCES += \
	RPC/Client/GpApiCliTransportHttp.cpp \
	RPC/Client/GpApiCliTransportHttpFactory.cpp \
	RPC/Server/GpApiSrvRequestHandlerHttp.cpp

HEADERS += \
	GpApiHttp.hpp \
	GpApiHttp_global.hpp \
	RPC/Client/GpApiCliTransportHttp.hpp \
	RPC/Client/GpApiCliTransportHttpFactory.hpp \
	RPC/Client/GpApiClientHttp.hpp \
	RPC/GpApiHttpRPC.hpp \
	RPC/Server/GpApiSrvRequestHandlerHttp.hpp
