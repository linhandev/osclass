#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x509eddf0, "module_layout" },
	{ 0x6439e797, "class_unregister" },
	{ 0x239d8204, "device_destroy" },
	{ 0x7bf2018e, "class_destroy" },
	{ 0xc7b7b1b, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xe90ba3f6, "__class_create" },
	{ 0x940f4bc0, "__register_chrdev" },
	{ 0x6ce54927, "_copy_to_user" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x56470118, "__warn_printk" },
	{ 0xa916b694, "strnlen" },
	{ 0x89cfd022, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2587B33DA5DDE73E7C42AF3");
