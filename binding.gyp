{
  'variables': {
    'platform': '<(OS)',
  },

  'targets': [
    {
      'target_name': 'boosh',
      'dependencies' : [
        'glfw',
        'glew',
        'node_modules/canvas/binding.gyp:canvas'
      ],
      'sources' : [
        'src/binding.cc',
        'src/window.cc'
      ],
      'include_dirs' : [
        'deps/glfw/include',
        'node_modules/canvas/src',
        'deps/glew/include'
      ]
    },
    {
      'target_name' : 'glew',
      'type' : 'static_library',
      'sources' : [
        'deps/glew/src/glew.c',
        'deps/glew/src/glewinfo.c',
        'deps/glew/src/visualinfo.c'
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
            '__MACOSX_CORE__'
          ],
          'link_settings' : {
            'libraries' : [
              '-framework OpenGL',
              '-framework Cocoa',
              '-framework IOKit'
            ]
          }
        }],
        ['platform == "win"', {'variables': {'platform': 'win32'}}],
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