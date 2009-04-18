/*
 * Copyright © 2009 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdint.h>
#include <X11/Xlibint.h>
#include <X11/extensions/XI2proto.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/extutil.h>
#include "XIint.h"

Status
XIQueryVersion(Display *dpy, int *major_return, int *minor_return)
{
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_xiQueryVersion(dpy, major_return, minor_return, info) != BadRequest) {
	UnlockDisplay(dpy);
	SyncHandle();
    }
}

_X_HIDDEN Status
_xiQueryVersion(Display * dpy, int *major, int *minor, XExtDisplayInfo *info)
{
    xXIQueryVersionReq *req;
    xXIQueryVersionReply rep;

    if (_XiCheckExtInit(dpy, Dont_Check, info) == -1)
	return BadRequest;

    GetReq(XIQueryVersion, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIQueryVersion;
    req->major_version = *major;
    req->minor_version = *minor;

    if (!_XReply(dpy, (xReply*)&rep, 0, xTrue)) {
	return BadImplementation;
    }
    *major = rep.major_version;
    *minor = rep.minor_version;
    return Success;
}
