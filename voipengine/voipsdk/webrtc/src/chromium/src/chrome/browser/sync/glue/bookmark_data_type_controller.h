// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BOOKMARK_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_BOOKMARK_DATA_TYPE_CONTROLLER_H__

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/sync/glue/frontend_data_type_controller.h"
#include "components/bookmarks/browser/base_bookmark_model_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace browser_sync {

// A class that manages the startup and shutdown of bookmark sync.
class BookmarkDataTypeController : public FrontendDataTypeController,
                                   public content::NotificationObserver,
                                   public bookmarks::BaseBookmarkModelObserver {
 public:
  BookmarkDataTypeController(ProfileSyncComponentsFactory* profile_sync_factory,
                             Profile* profile,
                             ProfileSyncService* sync_service);

  // FrontendDataTypeController:
  syncer::ModelType type() const override;

  // content::NotificationObserver:
  void Observe(int type,
               const content::NotificationSource& source,
               const content::NotificationDetails& details) override;

 private:
  ~BookmarkDataTypeController() override;

  // FrontendDataTypeController:
  bool StartModels() override;
  void CleanUpState() override;
  void CreateSyncComponents() override;

  // bookmarks::BaseBookmarkModelObserver:
  void BookmarkModelChanged() override;
  void BookmarkModelLoaded(BookmarkModel* model, bool ids_reassigned) override;
  void BookmarkModelBeingDeleted(BookmarkModel* model) override;

  // Helper that returns true iff both the bookmark model and the history
  // service have finished loading.
  bool DependentsLoaded();

  content::NotificationRegistrar registrar_;

  BookmarkModel* bookmark_model_;

  // Have we installed ourselves as a BookmarkModel observer?
  bool installed_bookmark_observer_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkDataTypeController);
};

}  // namespace browser_sync

#endif  // CHROME_BROWSER_SYNC_GLUE_BOOKMARK_DATA_TYPE_CONTROLLER_H__
