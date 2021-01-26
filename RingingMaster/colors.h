// To hold the colours and their names
typedef struct {
    COLORREF crColour;
    TCHAR    *szName;
} ColourTableEntry;

static ColourTableEntry crColours[] = 
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")				},
    { RGB(0x80, 0x00, 0x00),    _T("Dark Red")			},
    { RGB(0x00, 0x80, 0x00),    _T("Dark Green")		},
    { RGB(0x80, 0x80, 0x00),    _T("Dark Yellow")		},
    { RGB(0x00, 0x00, 0x80),    _T("Dark Blue")			},

    { RGB(0x80, 0x00, 0x80),    _T("Violet")			},
    { RGB(0x00, 0x80, 0x80),    _T("Dark Green")		},
    { RGB(0xC0, 0xC0, 0xC0),    _T("Gray-25%")			},
    { RGB(0xC0, 0xDC, 0xC0),    _T("Pale Green")		},
    { RGB(0xA6, 0xCA, 0xF0),	_T("Pale Blue")         },
    
    { RGB(0xFF, 0xFB, 0xF0),	_T("Parchment")         },
    { RGB(0xA0, 0xA0, 0xA4),	_T("Gray-30%")          },
    { RGB(0x80, 0x80, 0x80),	_T("Gray-50%")          },
    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0x00, 0xFF, 0x00),    _T("Bright Green")      },
	
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },    
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0xFF, 0x00, 0xFF),    _T("Pink")              },
    { RGB(0x00, 0xFF, 0xFF),    _T("Turquoise")         },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             },

	
	{ RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark Blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Gray-80%")          },
    { RGB(0x8B, 0x00, 0x00),    _T("Dark Red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark Yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-Gray")         },
    { RGB(0x66, 0x66, 0x66),    _T("Gray-50%")          },
    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light Orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea Green")         },
    { RGB(51,   204,  204),		_T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light Blue")        },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Gray-40%")          },
    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0,    204,  255 ),	_T("Sky Blue")          },
    { RGB(234,  128,  102 ),	_T("Plum")              },
    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(255,  254,  153 ),	_T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light Yellow")      },
    { RGB(204,  255,  204 ),	_T("Light Green ")      },
    { RGB(204,  255,  255 ),	_T("Light Turquoise")	},
    { RGB(153,  204,  255 ),	_T("Pale Blue")         },
    { RGB(204,  153,  255 ),	_T("Lavender")          },
};


// Initialize color names:
void InitColorNames	()
{
	int m_nNumColours = sizeof (crColours)/sizeof(ColourTableEntry);

	for (int i = 0; i < m_nNumColours; i++)
	{
		CBCGPColorMenuButton::SetColorName (crColours[i].crColour, crColours[i].szName);
	}
}



