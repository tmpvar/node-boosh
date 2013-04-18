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
        'src/binding.cc'
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
            'deps/glfw/lib/cocoa/cocoa_enable.m',
            'deps/glfw/lib/cocoa/cocoa_fullscreen.m',
            'deps/glfw/lib/cocoa/cocoa_glext.m',
            'deps/glfw/lib/cocoa/cocoa_init.m',
            'deps/glfw/lib/cocoa/cocoa_joystick.m',
            'deps/glfw/lib/cocoa/cocoa_thread.c',
            'deps/glfw/lib/cocoa/cocoa_time.m',
            'deps/glfw/lib/cocoa/cocoa_window.m'
          ],
          'include_dirs' : ['deps/glfw/lib/cocoa'],
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
        'deps/glfw/include',
        'deps/glfw/lib',
      ],
      'sources' : [
        'deps/glfw/lib/enable.c',
        'deps/glfw/lib/fullscreen.c',
        'deps/glfw/lib/glext.c',
        'deps/glfw/lib/image.c',
        'deps/glfw/lib/init.c',
        'deps/glfw/lib/input.c',
        'deps/glfw/lib/joystick.c',
        'deps/glfw/lib/stream.c',
        'deps/glfw/lib/tga.c',
        'deps/glfw/lib/thread.c',
        'deps/glfw/lib/time.c',
        'deps/glfw/lib/window.c'
      ]
    }
  ]
}