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
        'node_modules/glfw3/binding.gyp:glfw3-static',
        'node_modules/glew/binding.gyp:glew-static',
        'node_modules/context2d/binding.gyp:context2d-static',
        'node_modules/node-webgl/binding.gyp:webgl-static'
      ],
      'sources' : [
        'src/binding.cc',
        'src/window.cc'
      ],

      'include_dirs' : [
        '<@(shared_include_dirs)',
        'deps/glfw/include',
        'node_modules/context2d/src',
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
        }]
      ]
    }
  ]
}
