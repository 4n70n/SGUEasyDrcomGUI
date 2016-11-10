#SGUEasyDrcom-MacOS
SGUEasyDrcom-MacOS is a 3rd party Dr.COM Client for Shaoguan University based on Shindo's Project **[EasyDrcom](https://github.com/coverxit/EasyDrcom)**

## Introduction
Mac OS 版 SGUEasyDrcom-MacOS（以下简称**本项目**）使用 XCode 8.1 开发，语言为 C++（包括C++ 11的部分特性）和 Objective-C。

本项目依赖于 **libpcap**（已内建于Mac OS X），此外使用了第三方库 [STPrivilegedTask](https://github.com/sveinbjornt/STPrivilegedTask)。

## Thanks
本项目以Shindo创作的EasyDrcom修改重构得来，在此特别感谢Shindo的付出。
[原版代码在此](https://github.com/coverxit/EasyDrcomGUI)
**另外特别感谢Kit Tse的指导修改**

##另外特别引用Kit Tse的一段话
本目录会在2016.10.31或更后的时间呈现给城市热点公司的开发工程师。
因为不能直接沟通，所以我留下一点话，想告诉clone或fork或者只是watch的工程师（们）：

EasyDrcomCore的模拟登录就不用看了，我们目前仍然还没解决心跳的问题。好好改进GUI吧，虽然我觉得GUI部分看了我们第三方的代码之后仍然不会有太多帮助（认真脸）。Anyway，如果能够悄悄告诉我方法，让我们自己维护也行啊！（感觉苹果大更新一次，程序就要重新用最新版xcode编译一次）

## License
EasyDrcomGUI License:

	Copyright (C) 2014, 2015 Shindo 
	
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
	
		http://www.apache.org/licenses/LICENSE-2.0
	
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissionss and
	limitations under the License.

libpcap License:

	License: BSD
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	
	  1. Redistributions of source code must retain the above copyright
	     notice, this list of conditions and the following disclaimer.
	  2. Redistributions in binary form must reproduce the above copyright
	     notice, this list of conditions and the following disclaimer in
	     the documentation and/or other materials provided with the
	     distribution.
	  3. The names of the authors may not be used to endorse or promote
	     products derived from this software without specific prior
	     written permission.
	
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

STPrivilegedTask License:

	STPrivilegedTask - NSTask-like wrapper around AuthorizationExecuteWithPrivileges
	Copyright (C) 2009-2015 Sveinbjorn Thordarson <sveinbjornt@gmail.com>
	 
	BSD License
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	     * Redistributions of source code must retain the above copyright
	       notice, this list of conditions and the following disclaimer.
	     * Redistributions in binary form must reproduce the above copyright
	       notice, this list of conditions and the following disclaimer in the
	       documentation and/or other materials provided with the distribution.
	     * Neither the name of Sveinbjorn Thordarson nor that of any other
	       contributors may be used to endorse or promote products
	       derived from this software without specific prior written permission.
	  
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL  BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
