{
  'variables': {
    'platform': '<(OS)',
  },

  'includes' : {
    'node_modules/context2d/context2d.gypi'
  },

  'targets': [
    {
      'target_name': 'boosh',
      'dependencies' : [
        'glfw',
        'glew',
        'node_modules/context2d/binding.gyp:context2d-static'
      ],
      'sources' : [
        'src/binding.cc',
        'src/window.cc'
      ],

      'libraries' : [
        'opengl32.lib', 'glu32.lib'
      ],

      'include_dirs' : [
        '<@(shared_include_dirs)',
        'deps/glfw/include',
        'node_modules/context2d/src',
        'deps/glew/include'
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'CompileAs': 2, 
          'RuntimeLibrary': '0',
          'WarningLevel': '3',
          'ProgramDataBaseFileName': '$(OutDir)\\$(ProjectName).pdb',
          'DebugInformationFormat': '3',
          'ExceptionHandling': '0',
          'LinkIncremental': '0',
          'AdditionalOptions': [ '/MP', ]
        },
        'VCLinkerTool' : {
          'LinkIncremental' : '0',
          'LinkTimeCodeGeneration': '1',
          'AdditionalOptions': [ '/FORCE:MULTIPLE', ]
        }
      }
    },
    {
      'target_name' : 'glew',
      'type' : 'static_library',
      'sources' : [
        'deps/glew/src/glew.c'
      ],
      'include_dirs' : [
        'deps/glew/include'
      ]
    },
    {
      'target_name' : 'glfw',
      'type' : 'static_library',
      'conditions': [
        ['OS == "mac"', {
          'sources': [
            'deps/glfw/src/cocoa_clipboard.m',
            'deps/glfw/src/cocoa_gamma.c',
            'deps/glfw/src/cocoa_init.m',
            'deps/glfw/src/cocoa_joystick.m',
            'deps/glfw/src/cocoa_monitor.m',
            'deps/glfw/src/cocoa_time.c',
            'deps/glfw/src/cocoa_window.m',
            'deps/glfw/src/nsgl_context.m',
          ],
          'include_dirs' : [
            'deps/glfw/src',
            'deps/glfw-support/cocoa'
          ],
          'defines' : [
            '__MACOSX_CORE__',
            '_GLFW_USE_CONFIG_H'
          ],
          'link_settings' : {
            'libraries' : [
              '-framework OpenGL',
              '-framework Cocoa',
              '-framework IOKit'
            ]
          }
        }],
        ['platform == "win"', {
          'defines' : [
            # need angle to get this working
            #'_GLFW_USE_GLESV2',
            '_GLFW_USE_OPENGL',
            '_GLFW_WIN32',
            '_GLFW_WGL'
          ],
          'variables': {'platform': 'win32'},
          'sources' : [
            'deps/glfw/src/win32_clipboard.c',
            'deps/glfw/src/win32_gamma.c',
            'deps/glfw/src/win32_init.c',
            'deps/glfw/src/win32_joystick.c',
            'deps/glfw/src/win32_monitor.c',
            'deps/glfw/src/win32_time.c',
            'deps/glfw/src/win32_window.c',
            'deps/glfw/src/wgl_context.c',
          ],
          'include_dirs' : [
            'deps/glfw/src',
            'deps/glfw-support/win'
          ]
        }],
        ['platform == "linux"', {'variables': {'platform': 'x11'}}],
      ],

      'include_dirs' : [
        'deps/glfw/include'
      ],
      'sources' : [
        'deps/glfw/src/clipboard.c',
        'deps/glfw/src/context.c',
        'deps/glfw/src/gamma.c',
        'deps/glfw/src/init.c',
        'deps/glfw/src/input.c',
        'deps/glfw/src/joystick.c',
        'deps/glfw/src/monitor.c',
        'deps/glfw/src/time.c',
        'deps/glfw/src/window.c'
      ]
    }
  ]
}