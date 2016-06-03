{
  'variables': {
    'platform': '<(OS)',
  },

  'targets': [
    {
      'target_name': 'boosh',
      'sources' : [
        'src/binding.cc',
        'src/window.cc',
        'src/node-nanovg.cc',
        'deps/nanovg/src/nanovg.c'
      ],

      'libraries': [
        "../node_modules/glfw3/build/Release/glfw3-static.a",
        "../node_modules/glew/build/Release/glew-static.a",
      ],

      'include_dirs' : [
        "<!(node -e \"require('nan')\")",
        'node_modules/glew/deps/glew/include',
        'node_modules/glfw3/deps/glfw/include',
        'deps/nanovg/src/',
        'src'
      ],

      'conditions' : [
        ['OS == "win"', {
          'libraries' : [
            'opengl32.lib', 'glu32.lib'
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
        }],
        [ 'OS=="mac"', {
          'defines': [ '_DARWIN_USE_64_BIT_INODE=1' ],
          'libraries': [ '-undefined dynamic_lookup' ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
              '$(SDKROOT)/System/Library/Frameworks/CoreGraphics.framework',
              '$(SDKROOT)/System/Library/Frameworks/CoreText.framework',
              '$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
              '$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
              '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
              '$(SDKROOT)/System/Library/Frameworks/ImageIO.framework',
              '$(SDKROOT)/System/Library/Frameworks/ApplicationServices.framework',
              '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
            ],
          },

          'xcode_settings': {
            'DYLIB_INSTALL_NAME_BASE': '@rpath',
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
            'OTHER_CFLAGS': ['-stdlib=libc++']
          },
        }],
      ]
    }
  ]
}
