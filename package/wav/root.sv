#include "wav.h"
#include "wav.func.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	rl	{
	root	-export	.
	// object
	info	vlist	0	r	{
		author	string	0	r	AUTHOR
		package	string	0	r	PACKAGE
		version	vlist	0	r	{
			major	ubyte	0	r	PACKAGE_major
			minor	ubyte	0	r	PACKAGE_minor
			rev	ubyte	0	r	PACKAGE_revision
		}
	}
	limit	vlist	0	r	{
		// new
		maxtime	float	0	rw	_def_maxtime
		maxtime_phy	float	0	r	_phy_maxtime
		_lim_maxtime	-export	.limit.maxtime
		// noise: 最大迭代次数 最大迭代比例 终止迭代误差
		noise_ni	ubyte	0	rw	20
		noise_ri	float	0	rw	0.05
		noise_ei	float	0	rw	0.01
		_lim_noise_ni	-export	.limit.noise_ni
		_lim_noise_ri	-export	.limit.noise_ri
		_lim_noise_ei	-export	.limit.noise_ei
	}
	loudmode	vlist	0	r	{
		value	ubyte	0	rw	loudmode_ra
		_loudmode_	-export	.loudmode.value
		no	ubyte	0	r	loudmode_no
		ra	ubyte	0	r	loudmode_ra
		db_atte	ubyte	0	r	loudmode_db_atte
	}
	// data
	type	string	0	r	PACKAGE_type
	type	-export .type
	SamFre	slong	0	r	SamFre
	// func
	new	fun	0	r	addr_fun(new)
	write	fun	0	r	addr_fun(write)
	loudness	fun	0	r	addr_fun(loudness)
	loudexpe	fun	0	r	addr_fun(loudexpe)
	clear	fun	0	r	addr_fun(clear)
	rsin_inc	fun	0	r	addr_fun(rsin_inc)
	rcos_inc	fun	0	r	addr_fun(rcos_inc)
	rsin_get	fun	0	r	addr_fun(rsin_get)
	rcos_get	fun	0	r	addr_fun(rcos_get)
	
	aloud	fun	0	r	addr_fun(aloud)
	noise	fun	0	r	addr_fun(noise)
}

