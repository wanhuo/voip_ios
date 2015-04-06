// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/chrome_event_processing_window.h"

class AppNotificationBridge;

namespace info_bubble {

enum AnimationMask {
  kAnimateNone = 0,
  kAnimateOrderIn = 1 << 1,
  kAnimateOrderOut = 1 << 2,
};
typedef NSUInteger AllowedAnimations;

}  // namespace info_bubble

// A rounded window with an arrow used for example when you click on the STAR
// button or that pops up within our first-run UI.
@interface InfoBubbleWindow : ChromeEventProcessingWindow {
 @private
  // Is self in the process of closing.
  BOOL closing_;

  // Specifies if window order in and order out animations are allowed. By
  // default both types of animations are allowed.
  info_bubble::AllowedAnimations allowedAnimations_;

  // If NO the window will never become key.
  // Default YES.
  BOOL canBecomeKeyWindow_;

  // If NO the window will not share key state with its parent. Defaults to YES.
  // Can be set both by external callers, but is also changed internally, in
  // response to resignKeyWindow and becomeKeyWindow events.
  BOOL allowShareParentKeyState_;

  // Bridge to proxy Chrome notifications to the window.
  scoped_ptr<AppNotificationBridge> notificationBridge_;
}

@property(nonatomic) info_bubble::AllowedAnimations allowedAnimations;
@property(nonatomic) BOOL canBecomeKeyWindow;
@property(nonatomic) BOOL allowShareParentKeyState;

// Returns YES if the window is in the process of closing.
// Can't use "windowWillClose" notification because that will be sent
// after the closing animation has completed.
- (BOOL)isClosing;

@end
