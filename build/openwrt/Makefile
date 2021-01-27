include $(TOPDIR)/rules.mk

PKG_NAME:=simple_net_camera
PKG_VERSION:=0.0.1
PKG_RELEASE:=1
PKG_INSTALL:=1
PKG_MAINTAINER:=nejidev@yahoo.co.jp

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/nejidev/simple_net_camera.git
PKG_MIRROR_HASH:=1f453a1f10f6f3c2a07c9b175a8c987585cc4ea01b3ff8c1f77324a3a07ff3c3
PKG_SOURCE_VERSION:=a3412174928a30144756443a82358034dd9ab35d
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=simple net camera
  URL:=https://github.com/nejidev/simple_net_camera/
  DEPENDS:=+libcurl +libstdcpp
endef

define Build/Configure
	echo $(PKG_BUILD_DIR)
	echo $(PKG_BUILD_DIR)/../$(PKG_SOURCE_SUBDIR)
	cp $(PKG_BUILD_DIR)/../$(PKG_SOURCE_SUBDIR)/src $(PKG_BUILD_DIR)/ -rf
	cp $(PKG_BUILD_DIR)/../$(PKG_SOURCE_SUBDIR)/include $(PKG_BUILD_DIR)/ -rf
	cp $(PKG_BUILD_DIR)/../$(PKG_SOURCE_SUBDIR)/build $(PKG_BUILD_DIR)/ -rf
	cp $(PKG_BUILD_DIR)/build/Makefile $(PKG_BUILD_DIR)/ -rf
	sed -i 's/\.\.\///g' $(PKG_BUILD_DIR)/Makefile
	sed -i '19d' $(PKG_BUILD_DIR)/Makefile
	sed -i '19d' $(PKG_BUILD_DIR)/Makefile
	sed -i 's/all:/install:/' $(PKG_BUILD_DIR)/Makefile
endef

define Build/Compile
endef

define Build/Prepare
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/simple_net_camera $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

