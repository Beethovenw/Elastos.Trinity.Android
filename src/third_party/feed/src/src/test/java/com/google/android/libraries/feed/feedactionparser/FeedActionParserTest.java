// Copyright 2018 The Feed Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.google.android.libraries.feed.feedactionparser;

import static com.google.common.truth.Truth.assertThat;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import com.google.android.libraries.feed.host.action.StreamActionApi;
import com.google.search.now.ui.action.FeedActionPayloadProto.FeedActionPayload;
import com.google.search.now.ui.action.FeedActionProto.DownloadData;
import com.google.search.now.ui.action.FeedActionProto.FeedAction;
import com.google.search.now.ui.action.FeedActionProto.FeedActionMetadata;
import com.google.search.now.ui.action.FeedActionProto.FeedActionMetadata.Type;
import com.google.search.now.ui.action.FeedActionProto.LabelledFeedActionData;
import com.google.search.now.ui.action.FeedActionProto.OpenContextMenuData;
import com.google.search.now.ui.action.FeedActionProto.OpenUrlData;
import com.google.search.now.ui.action.PietExtensionsProto.PietFeedActionPayload;
import com.google.search.now.ui.piet.ActionsProto.Action;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricTestRunner;

/** Tests for {@link FeedActionParser}. */
@RunWith(RobolectricTestRunner.class)
public class FeedActionParserTest {
  public static final String URL = "www.google.com";

  private static final FeedActionPayload OPEN_URL_FEED_ACTION =
      FeedActionPayload.newBuilder()
          .setExtension(
              FeedAction.feedActionExtension,
              FeedAction.newBuilder()
                  .setMetadata(
                      FeedActionMetadata.newBuilder()
                          .setType(Type.OPEN_URL)
                          .setOpenUrlData(OpenUrlData.newBuilder().setUrl(URL)))
                  .build())
          .build();
  private static final FeedActionPayload CONTEXT_MENU_FEED_ACTION =
      FeedActionPayload.newBuilder()
          .setExtension(
              FeedAction.feedActionExtension,
              FeedAction.newBuilder()
                  .setMetadata(
                      FeedActionMetadata.newBuilder()
                          .setType(Type.OPEN_CONTEXT_MENU)
                          .setOpenContextMenuData(
                              OpenContextMenuData.newBuilder()
                                  .addContextMenuData(
                                      LabelledFeedActionData.newBuilder()
                                          .setLabel("Open url")
                                          .setFeedActionPayload(OPEN_URL_FEED_ACTION))))
                  .build())
          .build();

  private static final FeedActionPayload OPEN_URL_NEW_WINDOW_FEED_ACTION =
      FeedActionPayload.newBuilder()
          .setExtension(
              FeedAction.feedActionExtension,
              FeedAction.newBuilder()
                  .setMetadata(
                      FeedActionMetadata.newBuilder()
                          .setType(Type.OPEN_URL_NEW_WINDOW)
                          .setOpenUrlData(OpenUrlData.newBuilder().setUrl(URL)))
                  .build())
          .build();

  private static final FeedActionPayload OPEN_URL_INCOGNITO_FEED_ACTION =
      FeedActionPayload.newBuilder()
          .setExtension(
              FeedAction.feedActionExtension,
              FeedAction.newBuilder()
                  .setMetadata(
                      FeedActionMetadata.newBuilder()
                          .setType(Type.OPEN_URL_INCOGNITO)
                          .setOpenUrlData(OpenUrlData.newBuilder().setUrl(URL)))
                  .build())
          .build();

  private static final FeedActionPayload DOWNLOAD_URL_FEED_ACTION =
      FeedActionPayload.newBuilder()
          .setExtension(
              FeedAction.feedActionExtension,
              FeedAction.newBuilder()
                  .setMetadata(
                      FeedActionMetadata.newBuilder()
                          .setType(Type.DOWNLOAD)
                          .setDownloadData(DownloadData.newBuilder().setUrl(URL)))
                  .build())
          .build();
  private static final FeedActionPayload DOWNLOAD_URL_FEED_ACTION_NO_URL =
      FeedActionPayload.newBuilder()
          .setExtension(
              FeedAction.feedActionExtension,
              FeedAction.newBuilder()
                  .setMetadata(
                      FeedActionMetadata.newBuilder()
                          .setType(Type.DOWNLOAD)
                          .setDownloadData(DownloadData.newBuilder()))
                  .build())
          .build();

  private static final Action OPEN_URL_ACTION =
      Action.newBuilder()
          .setExtension(
              PietFeedActionPayload.pietFeedActionPayloadExtension,
              PietFeedActionPayload.newBuilder().setFeedActionPayload(OPEN_URL_FEED_ACTION).build())
          .build();

  private static final Action OPEN_INCOGNITO_ACTION =
      Action.newBuilder()
          .setExtension(
              PietFeedActionPayload.pietFeedActionPayloadExtension,
              PietFeedActionPayload.newBuilder()
                  .setFeedActionPayload(OPEN_URL_INCOGNITO_FEED_ACTION)
                  .build())
          .build();

  private static final Action OPEN_NEW_WINDOW_ACTION =
      Action.newBuilder()
          .setExtension(
              PietFeedActionPayload.pietFeedActionPayloadExtension,
              PietFeedActionPayload.newBuilder()
                  .setFeedActionPayload(OPEN_URL_NEW_WINDOW_FEED_ACTION)
                  .build())
          .build();

  @Mock private StreamActionApi streamActionApi;

  private final FeedActionParser feedActionParser = new FeedActionParser();

  @Before
  public void setup() {
    initMocks(this);
  }

  @Test
  public void testParseAction() {
    when(streamActionApi.canOpenUrl()).thenReturn(true);
    feedActionParser.parseAction(
        OPEN_URL_ACTION, streamActionApi, /* view= */ null, /* veLoggingToken= */ null);

    verify(streamActionApi).openUrl(URL);
  }

  @Test
  public void testParseAction_newWindow() {
    when(streamActionApi.canOpenUrlInNewWindow()).thenReturn(true);
    feedActionParser.parseAction(
        OPEN_NEW_WINDOW_ACTION, streamActionApi, /* view= */ null, /* veLoggingToken= */ null);

    verify(streamActionApi).openUrlInNewWindow(URL);
  }

  @Test
  public void testParseAction_incognito() {
    when(streamActionApi.canOpenUrlInIncognitoMode()).thenReturn(true);
    feedActionParser.parseAction(
        OPEN_INCOGNITO_ACTION, streamActionApi, /* view= */ null, /* veLoggingToken= */ null);

    verify(streamActionApi).openUrlInIncognitoMode(URL);
  }

  @Test
  public void testParseAction_contextMenu() {
    Context context = Robolectric.setupActivity(Activity.class);
    View view = new View(context);

    when(streamActionApi.canOpenContextMenu()).thenReturn(true);
    PietFeedActionPayload contextMenuPietFeedAction =
        PietFeedActionPayload.newBuilder().setFeedActionPayload(CONTEXT_MENU_FEED_ACTION).build();

    feedActionParser.parseAction(
        Action.newBuilder()
            .setExtension(
                PietFeedActionPayload.pietFeedActionPayloadExtension, contextMenuPietFeedAction)
            .build(),
        streamActionApi,
        /* view= */ view,
        /* veLoggingToken= */ null);

    verify(streamActionApi)
        .openContextMenu(
            contextMenuPietFeedAction
                .getFeedActionPayload()
                .getExtension(FeedAction.feedActionExtension)
                .getMetadata()
                .getOpenContextMenuData(),
            view);
  }

  @Test
  public void testCanPerformActionFromContextMenu_openUrl() {
    when(streamActionApi.canOpenUrl()).thenReturn(true);
    assertThat(feedActionParser.canPerformAction(OPEN_URL_FEED_ACTION, streamActionApi)).isTrue();
    verify(streamActionApi).canOpenUrl();
    verifyNoMoreInteractions(streamActionApi);
  }

  @Test
  public void testCanPerformActionFromContextMenu_newWindow() {
    when(streamActionApi.canOpenUrlInNewWindow()).thenReturn(true);
    assertThat(feedActionParser.canPerformAction(OPEN_URL_NEW_WINDOW_FEED_ACTION, streamActionApi))
        .isTrue();
    verify(streamActionApi).canOpenUrlInNewWindow();
    verifyNoMoreInteractions(streamActionApi);
  }

  @Test
  public void testCanPerformActionFromContextMenu_incognito() {
    when(streamActionApi.canOpenUrlInIncognitoMode()).thenReturn(true);
    assertThat(feedActionParser.canPerformAction(OPEN_URL_INCOGNITO_FEED_ACTION, streamActionApi))
        .isTrue();
    verify(streamActionApi).canOpenUrlInIncognitoMode();
    verifyNoMoreInteractions(streamActionApi);
  }

  @Test
  public void testCanPerformActionFromContextMenu_nestedContextMenu() {
    when(streamActionApi.canOpenContextMenu()).thenReturn(true);

    assertThat(feedActionParser.canPerformAction(CONTEXT_MENU_FEED_ACTION, streamActionApi))
        .isTrue();
    verify(streamActionApi).canOpenContextMenu();
    verifyNoMoreInteractions(streamActionApi);
  }

  @Test
  public void testCanPerformActionFromContextMenu_downloadUrl() {
    when(streamActionApi.canDownloadUrl()).thenReturn(true);
    assertThat(feedActionParser.canPerformAction(DOWNLOAD_URL_FEED_ACTION, streamActionApi))
        .isTrue();
    verify(streamActionApi).canDownloadUrl();
    verifyNoMoreInteractions(streamActionApi);
  }

  @Test
  public void testDownloadUrl_noUrlPresent() {
    when(streamActionApi.canDownloadUrl()).thenReturn(true);
    feedActionParser.parseFeedActionPayload(
        DOWNLOAD_URL_FEED_ACTION_NO_URL, streamActionApi, /* view= */ null);
    verify(streamActionApi, times(0)).downloadUrl(anyString());
  }

  @Test
  public void testDownloadUrl_noHostSupport() {
    when(streamActionApi.canDownloadUrl()).thenReturn(false);
    feedActionParser.parseFeedActionPayload(
        DOWNLOAD_URL_FEED_ACTION, streamActionApi, /* view= */ null);
    verify(streamActionApi, times(0)).downloadUrl(anyString());
  }
}
