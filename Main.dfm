object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'ClusterSearch'
  ClientHeight = 345
  ClientWidth = 530
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PathEdit: TLabeledEdit
    Left = 8
    Top = 24
    Width = 513
    Height = 21
    EditLabel.Width = 79
    EditLabel.Height = 13
    EditLabel.Caption = #1055#1091#1090#1100' '#1082' '#1088#1072#1079#1076#1077#1083#1091
    TabOrder = 0
    Text = '\\.\C:'
  end
  object ResultTree: TVirtualStringTree
    Left = 8
    Top = 96
    Width = 273
    Height = 241
    Header.AutoSizeIndex = 0
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.Options = [hoAutoResize, hoColumnResize, hoDrag, hoShowHint, hoShowSortGlyphs, hoVisible]
    TabOrder = 1
    TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowTreeLines, toShowVertGridLines, toThemeAware, toUseBlendedImages]
    Columns = <
      item
        Position = 0
        Width = 169
        WideText = 'id'
      end
      item
        Position = 1
        WideText = 'inode'
      end
      item
        Position = 2
        WideText = 'format'
      end>
  end
  object SearchButton: TButton
    Left = 232
    Top = 51
    Width = 75
    Height = 25
    Caption = #1055#1086#1080#1089#1082'!'
    TabOrder = 2
    OnClick = SearchButtonClick
  end
end
