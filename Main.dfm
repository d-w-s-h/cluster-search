object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'ClusterSearch'
  ClientHeight = 349
  ClientWidth = 263
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object PathEdit: TLabeledEdit
    Left = 8
    Top = 24
    Width = 249
    Height = 21
    EditLabel.Width = 79
    EditLabel.Height = 13
    EditLabel.Caption = #1055#1091#1090#1100' '#1082' '#1088#1072#1079#1076#1077#1083#1091
    TabOrder = 0
    Text = '\\.\E:'
  end
  object ResultTree: TVirtualStringTree
    Left = 8
    Top = 96
    Width = 249
    Height = 241
    Header.AutoSizeIndex = 0
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.Height = 15
    Header.Options = [hoAutoResize, hoColumnResize, hoDrag, hoShowSortGlyphs, hoVisible, hoAutoSpring]
    ParentShowHint = False
    ScrollBarOptions.AlwaysVisible = True
    ScrollBarOptions.ScrollBars = ssVertical
    ScrollBarOptions.ScrollBarStyle = sbm3D
    ShowHint = False
    TabOrder = 1
    TreeOptions.AutoOptions = [toAutoDropExpand, toAutoExpand, toAutoScroll, toAutoScrollOnExpand, toAutoSort, toAutoSpanColumns, toAutoTristateTracking, toAutoHideButtons, toAutoDeleteMovedNodes, toDisableAutoscrollOnFocus, toAutoChangeScale, toAutoFreeOnCollapse, toDisableAutoscrollOnEdit]
    TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowVertGridLines, toThemeAware, toUseBlendedImages, toFullVertGridLines]
    OnGetText = ResultTreeGetText
    Columns = <
      item
        Position = 0
        Width = 44
        WideText = 'id'
      end
      item
        Position = 1
        Width = 77
        WideText = 'cluster'
      end
      item
        Position = 2
        Width = 107
        WideText = 'format'
      end>
  end
  object SearchButton: TButton
    Left = 8
    Top = 51
    Width = 121
    Height = 39
    Caption = #1055#1086#1080#1089#1082'!'
    TabOrder = 2
    OnClick = SearchButtonClick
  end
  object StopButton: TButton
    Left = 128
    Top = 51
    Width = 129
    Height = 39
    Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100
    TabOrder = 3
    OnClick = StopButtonClick
  end
end
