object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'ClusterSearch'
  ClientHeight = 423
  ClientWidth = 265
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object FSinfoLabel: TLabel
    Left = 8
    Top = 335
    Width = 3
    Height = 13
  end
  object PathEdit: TLabeledEdit
    Left = 8
    Top = 24
    Width = 249
    Height = 21
    EditLabel.Width = 79
    EditLabel.Height = 13
    EditLabel.Caption = #1055#1091#1090#1100' '#1082' '#1088#1072#1079#1076#1077#1083#1091
    TabOrder = 0
    Text = '\\.\G:'
  end
  object ResultTree: TVirtualStringTree
    Left = 8
    Top = 96
    Width = 249
    Height = 233
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
    TreeOptions.AutoOptions = [toAutoDropExpand, toAutoExpand, toAutoScroll, toAutoScrollOnExpand, toAutoSpanColumns, toAutoTristateTracking, toAutoHideButtons, toDisableAutoscrollOnFocus, toAutoChangeScale, toAutoFreeOnCollapse, toDisableAutoscrollOnEdit, toAutoBidiColumnOrdering]
    TreeOptions.ExportMode = emSelected
    TreeOptions.MiscOptions = [toAcceptOLEDrop, toCheckSupport, toEditable, toFullRepaintOnResize, toInitOnSave, toToggleOnDblClick, toWheelPanning, toEditOnClick]
    TreeOptions.PaintOptions = [toPopupMode, toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowVertGridLines, toThemeAware, toUseBlendedImages, toFullVertGridLines, toUseBlendedSelection, toUseExplorerTheme, toHideTreeLinesIfThemed]
    TreeOptions.SelectionOptions = [toDisableDrawSelection, toExtendedFocus, toFullRowSelect, toLevelSelectConstraint, toMiddleClickSelect, toMultiSelect, toCenterScrollIntoView]
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
    Enabled = False
    TabOrder = 3
    OnClick = StopButtonClick
  end
  object CheckPNG: TCheckBox
    Left = 216
    Top = 335
    Width = 41
    Height = 17
    Caption = 'png'
    TabOrder = 4
  end
  object CheckBMP: TCheckBox
    Left = 216
    Top = 358
    Width = 41
    Height = 17
    Caption = 'bmp'
    TabOrder = 5
  end
  object ProgressBar: TProgressBar
    Left = 128
    Top = 398
    Width = 129
    Height = 11
    Smooth = True
    Step = 1
    TabOrder = 6
    Visible = False
  end
end
