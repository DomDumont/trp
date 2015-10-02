
var Module;

if (typeof Module === 'undefined') Module = eval('(function() { try { return Module || {} } catch(e) { return {} } })()');

if (!Module.expectedDataFileDownloads) {
  Module.expectedDataFileDownloads = 0;
  Module.finishedDataFileDownloads = 0;
}
Module.expectedDataFileDownloads++;
(function() {
 var loadPackage = function(metadata) {

    var PACKAGE_PATH;
    if (typeof window === 'object') {
      PACKAGE_PATH = window['encodeURIComponent'](window.location.pathname.toString().substring(0, window.location.pathname.toString().lastIndexOf('/')) + '/');
    } else if (typeof location !== 'undefined') {
      // worker
      PACKAGE_PATH = encodeURIComponent(location.pathname.toString().substring(0, location.pathname.toString().lastIndexOf('/')) + '/');
    } else {
      throw 'using preloaded data can only be done on a web page or in a web worker';
    }
    var PACKAGE_NAME = 'trp.data';
    var REMOTE_PACKAGE_BASE = 'trp.data';
    if (typeof Module['locateFilePackage'] === 'function' && !Module['locateFile']) {
      Module['locateFile'] = Module['locateFilePackage'];
      Module.printErr('warning: you defined Module.locateFilePackage, that has been renamed to Module.locateFile (using your locateFilePackage for now)');
    }
    var REMOTE_PACKAGE_NAME = typeof Module['locateFile'] === 'function' ?
                              Module['locateFile'](REMOTE_PACKAGE_BASE) :
                              ((Module['filePackagePrefixURL'] || '') + REMOTE_PACKAGE_BASE);
  
      var REMOTE_PACKAGE_SIZE = 3370615;
      var PACKAGE_UUID = '6333226c-1389-43c3-83e5-6138bd971ce2';
    
    function fetchRemotePackage(packageName, packageSize, callback, errback) {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', packageName, true);
      xhr.responseType = 'arraybuffer';
      xhr.onprogress = function(event) {
        var url = packageName;
        var size = packageSize;
        if (event.total) size = event.total;
        if (event.loaded) {
          if (!xhr.addedTotal) {
            xhr.addedTotal = true;
            if (!Module.dataFileDownloads) Module.dataFileDownloads = {};
            Module.dataFileDownloads[url] = {
              loaded: event.loaded,
              total: size
            };
          } else {
            Module.dataFileDownloads[url].loaded = event.loaded;
          }
          var total = 0;
          var loaded = 0;
          var num = 0;
          for (var download in Module.dataFileDownloads) {
          var data = Module.dataFileDownloads[download];
            total += data.total;
            loaded += data.loaded;
            num++;
          }
          total = Math.ceil(total * Module.expectedDataFileDownloads/num);
          if (Module['setStatus']) Module['setStatus']('Downloading data... (' + loaded + '/' + total + ')');
        } else if (!Module.dataFileDownloads) {
          if (Module['setStatus']) Module['setStatus']('Downloading data...');
        }
      };
      xhr.onload = function(event) {
        var packageData = xhr.response;
        callback(packageData);
      };
      xhr.send(null);
    };

    function handleError(error) {
      console.error('package error:', error);
    };
  
      var fetched = null, fetchedCallback = null;
      fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE, function(data) {
        if (fetchedCallback) {
          fetchedCallback(data);
          fetchedCallback = null;
        } else {
          fetched = data;
        }
      }, handleError);
    
  function runWithFS() {

    function assert(check, msg) {
      if (!check) throw msg + new Error().stack;
    }
Module['FS_createPath']('/', 'helpers', true, true);
Module['FS_createPath']('/', 'themes', true, true);
Module['FS_createPath']('/themes', 'aeon', true, true);
Module['FS_createPath']('/themes/aeon', 'fonts', true, true);
Module['FS_createPath']('/themes/aeon', 'images', true, true);
Module['FS_createPath']('/themes', 'castlevania', true, true);
Module['FS_createPath']('/themes/castlevania', 'fonts', true, true);
Module['FS_createPath']('/themes/castlevania', 'images', true, true);
Module['FS_createPath']('/themes', 'metal', true, true);
Module['FS_createPath']('/themes/metal', 'fonts', true, true);
Module['FS_createPath']('/themes/metal', 'images', true, true);
Module['FS_createPath']('/themes', 'metroid', true, true);
Module['FS_createPath']('/themes/metroid', 'fonts', true, true);
Module['FS_createPath']('/themes/metroid', 'images', true, true);
Module['FS_createPath']('/themes', 'minimal', true, true);
Module['FS_createPath']('/themes/minimal', 'fonts', true, true);
Module['FS_createPath']('/themes/minimal', 'images', true, true);
Module['FS_createPath']('/', 'gamedata-sample', true, true);
Module['FS_createPath']('/gamedata-sample', 'fonts', true, true);
Module['FS_createPath']('/gamedata-sample', 'graphics', true, true);
Module['FS_createPath']('/gamedata-sample', 'particles', true, true);
Module['FS_createPath']('/gamedata-sample', 'sounds', true, true);
Module['FS_createPath']('/gamedata-sample', 'strings', true, true);

    function DataRequest(start, end, crunched, audio) {
      this.start = start;
      this.end = end;
      this.crunched = crunched;
      this.audio = audio;
    }
    DataRequest.prototype = {
      requests: {},
      open: function(mode, name) {
        this.name = name;
        this.requests[name] = this;
        Module['addRunDependency']('fp ' + this.name);
      },
      send: function() {},
      onload: function() {
        var byteArray = this.byteArray.subarray(this.start, this.end);

          this.finish(byteArray);

      },
      finish: function(byteArray) {
        var that = this;
        Module['FS_createPreloadedFile'](this.name, null, byteArray, true, true, function() {
          Module['removeRunDependency']('fp ' + that.name);
        }, function() {
          if (that.audio) {
            Module['removeRunDependency']('fp ' + that.name); // workaround for chromium bug 124926 (still no audio with this, but at least we don't hang)
          } else {
            Module.printErr('Preloading file ' + that.name + ' failed');
          }
        }, false, true); // canOwn this data in the filesystem, it is a slide into the heap that will never change
        this.requests[this.name] = null;
      },
    };

      new DataRequest(0, 518, 0, 0).open('GET', '/settings.xml');
    new DataRequest(518, 2180, 0, 0).open('GET', '/helpers/scene_manager.rsc');
    new DataRequest(2180, 3082, 0, 0).open('GET', '/helpers/trp_api.rsc');
    new DataRequest(3082, 4045, 0, 0).open('GET', '/themes/aeon/aeon.xml');
    new DataRequest(4045, 45065, 0, 0).open('GET', '/themes/aeon/fonts/aeon_main.ttf');
    new DataRequest(45065, 125329, 0, 0).open('GET', '/themes/aeon/fonts/pf_ronda_seven.ttf');
    new DataRequest(125329, 198193, 0, 0).open('GET', '/themes/aeon/fonts/pf_ronda_seven_bold.ttf');
    new DataRequest(198193, 215440, 0, 0).open('GET', '/themes/aeon/images/aeon.png');
    new DataRequest(215440, 224448, 0, 0).open('GET', '/themes/aeon/images/aeon.xml');
    new DataRequest(224448, 225351, 0, 0).open('GET', '/themes/castlevania/castlevania.xml');
    new DataRequest(225351, 290027, 0, 0).open('GET', '/themes/castlevania/fonts/lohengrin.ttf');
    new DataRequest(290027, 340115, 0, 0).open('GET', '/themes/castlevania/images/castlevania.png');
    new DataRequest(340115, 345905, 0, 0).open('GET', '/themes/castlevania/images/castlevania.xml');
    new DataRequest(345905, 346885, 0, 0).open('GET', '/themes/metal/metal.xml');
    new DataRequest(346885, 496393, 0, 0).open('GET', '/themes/metal/fonts/SourceSansPro-Regular.ttf');
    new DataRequest(496393, 645745, 0, 0).open('GET', '/themes/metal/fonts/SourceSansPro-Semibold.ttf');
    new DataRequest(645745, 680554, 0, 0).open('GET', '/themes/metal/images/metal.png');
    new DataRequest(680554, 685508, 0, 0).open('GET', '/themes/metal/images/metal.xml');
    new DataRequest(685508, 686405, 0, 0).open('GET', '/themes/metroid/metroid.xml');
    new DataRequest(686405, 695261, 0, 0).open('GET', '/themes/metroid/fonts/metroid.ttf');
    new DataRequest(695261, 722775, 0, 0).open('GET', '/themes/metroid/images/metroid.png');
    new DataRequest(722775, 728558, 0, 0).open('GET', '/themes/metroid/images/metroid.xml');
    new DataRequest(728558, 729462, 0, 0).open('GET', '/themes/minimal/minimal.xml');
    new DataRequest(729462, 809726, 0, 0).open('GET', '/themes/minimal/fonts/pf_ronda_seven.ttf');
    new DataRequest(809726, 882590, 0, 0).open('GET', '/themes/minimal/fonts/pf_ronda_seven_bold.ttf');
    new DataRequest(882590, 909408, 0, 0).open('GET', '/themes/minimal/images/minimal.png');
    new DataRequest(909408, 914441, 0, 0).open('GET', '/themes/minimal/images/minimal.xml');
    new DataRequest(914441, 915923, 0, 0).open('GET', '/gamedata-sample/animation_scene.rsc');
    new DataRequest(915923, 917502, 0, 0).open('GET', '/gamedata-sample/dialog_scene.rsc');
    new DataRequest(917502, 921401, 0, 0).open('GET', '/gamedata-sample/game_scene.rsc');
    new DataRequest(921401, 923664, 0, 0).open('GET', '/gamedata-sample/gui_scene.rsc');
    new DataRequest(923664, 925213, 0, 0).open('GET', '/gamedata-sample/helloworld_scene.rsc');
    new DataRequest(925213, 927444, 0, 0).open('GET', '/gamedata-sample/io_scene.rsc');
    new DataRequest(927444, 931180, 0, 0).open('GET', '/gamedata-sample/main.rsc');
    new DataRequest(931180, 936552, 0, 0).open('GET', '/gamedata-sample/menu_scene.rsc');
    new DataRequest(936552, 937866, 0, 0).open('GET', '/gamedata-sample/music_scene.rsc');
    new DataRequest(937866, 939575, 0, 0).open('GET', '/gamedata-sample/particle_scene.rsc');
    new DataRequest(939575, 943914, 0, 0).open('GET', '/gamedata-sample/physics_scene.rsc');
    new DataRequest(943914, 953542, 0, 0).open('GET', '/gamedata-sample/platform.tmx');
    new DataRequest(953542, 955113, 0, 0).open('GET', '/gamedata-sample/platform_scene.rsc');
    new DataRequest(955113, 956924, 0, 0).open('GET', '/gamedata-sample/primitive_scene.rsc');
    new DataRequest(956924, 957551, 0, 0).open('GET', '/gamedata-sample/project.rap');
    new DataRequest(957551, 958775, 0, 0).open('GET', '/gamedata-sample/sound_scene.rsc');
    new DataRequest(958775, 960505, 0, 0).open('GET', '/gamedata-sample/sprite_scene.rsc');
    new DataRequest(960505, 962256, 0, 0).open('GET', '/gamedata-sample/textbox_scene.rsc');
    new DataRequest(962256, 964024, 0, 0).open('GET', '/gamedata-sample/tween_scene.rsc');
    new DataRequest(964024, 988928, 0, 0).open('GET', '/gamedata-sample/fonts/casual.ttf');
    new DataRequest(988928, 1347184, 0, 0).open('GET', '/gamedata-sample/fonts/consola.ttf');
    new DataRequest(1347184, 1440648, 0, 0).open('GET', '/gamedata-sample/fonts/kinkie.ttf');
    new DataRequest(1440648, 1462392, 0, 0).open('GET', '/gamedata-sample/fonts/vitamin.ttf');
    new DataRequest(1462392, 2043190, 0, 0).open('GET', '/gamedata-sample/graphics/apple-bg.jpg');
    new DataRequest(2043190, 2094924, 0, 0).open('GET', '/gamedata-sample/graphics/back1.jpg');
    new DataRequest(2094924, 2626144, 0, 0).open('GET', '/gamedata-sample/graphics/broute.png');
    new DataRequest(2626144, 2633039, 0, 0).open('GET', '/gamedata-sample/graphics/broute.xml');
    new DataRequest(2633039, 2642018, 0, 0).open('GET', '/gamedata-sample/graphics/level1.tmx');
    new DataRequest(2642018, 2704388, 0, 0).open('GET', '/gamedata-sample/graphics/platform1.bmp');
    new DataRequest(2704388, 2720240, 0, 0).open('GET', '/gamedata-sample/graphics/Sheep.png');
    new DataRequest(2720240, 2744463, 0, 0).open('GET', '/gamedata-sample/graphics/sheet.png');
    new DataRequest(2744463, 2744873, 0, 0).open('GET', '/gamedata-sample/graphics/sheet.xml');
    new DataRequest(2744873, 2873708, 0, 0).open('GET', '/gamedata-sample/graphics/tiles_spritesheet.png');
    new DataRequest(2873708, 2887273, 0, 0).open('GET', '/gamedata-sample/graphics/tiles_spritesheet.xml');
    new DataRequest(2887273, 2887374, 0, 0).open('GET', '/gamedata-sample/particles/snow.xml');
    new DataRequest(2887374, 3027252, 0, 0).open('GET', '/gamedata-sample/sounds/invitation.mod');
    new DataRequest(3027252, 3049797, 0, 1).open('GET', '/gamedata-sample/sounds/pop.ogg');
    new DataRequest(3049797, 3068223, 0, 1).open('GET', '/gamedata-sample/sounds/ressort.ogg');
    new DataRequest(3068223, 3349995, 0, 1).open('GET', '/gamedata-sample/sounds/sample.ogg');
    new DataRequest(3349995, 3370306, 0, 1).open('GET', '/gamedata-sample/sounds/sheep.ogg');
    new DataRequest(3370306, 3370460, 0, 0).open('GET', '/gamedata-sample/strings/en.xml');
    new DataRequest(3370460, 3370615, 0, 0).open('GET', '/gamedata-sample/strings/fr.xml');

    function processPackageData(arrayBuffer) {
      Module.finishedDataFileDownloads++;
      assert(arrayBuffer, 'Loading data file failed.');
      var byteArray = new Uint8Array(arrayBuffer);
      var curr;
      
      // copy the entire loaded file into a spot in the heap. Files will refer to slices in that. They cannot be freed though
      // (we may be allocating before malloc is ready, during startup).
      var ptr = Module['getMemory'](byteArray.length);
      Module['HEAPU8'].set(byteArray, ptr);
      DataRequest.prototype.byteArray = Module['HEAPU8'].subarray(ptr, ptr+byteArray.length);
          DataRequest.prototype.requests["/settings.xml"].onload();
          DataRequest.prototype.requests["/helpers/scene_manager.rsc"].onload();
          DataRequest.prototype.requests["/helpers/trp_api.rsc"].onload();
          DataRequest.prototype.requests["/themes/aeon/aeon.xml"].onload();
          DataRequest.prototype.requests["/themes/aeon/fonts/aeon_main.ttf"].onload();
          DataRequest.prototype.requests["/themes/aeon/fonts/pf_ronda_seven.ttf"].onload();
          DataRequest.prototype.requests["/themes/aeon/fonts/pf_ronda_seven_bold.ttf"].onload();
          DataRequest.prototype.requests["/themes/aeon/images/aeon.png"].onload();
          DataRequest.prototype.requests["/themes/aeon/images/aeon.xml"].onload();
          DataRequest.prototype.requests["/themes/castlevania/castlevania.xml"].onload();
          DataRequest.prototype.requests["/themes/castlevania/fonts/lohengrin.ttf"].onload();
          DataRequest.prototype.requests["/themes/castlevania/images/castlevania.png"].onload();
          DataRequest.prototype.requests["/themes/castlevania/images/castlevania.xml"].onload();
          DataRequest.prototype.requests["/themes/metal/metal.xml"].onload();
          DataRequest.prototype.requests["/themes/metal/fonts/SourceSansPro-Regular.ttf"].onload();
          DataRequest.prototype.requests["/themes/metal/fonts/SourceSansPro-Semibold.ttf"].onload();
          DataRequest.prototype.requests["/themes/metal/images/metal.png"].onload();
          DataRequest.prototype.requests["/themes/metal/images/metal.xml"].onload();
          DataRequest.prototype.requests["/themes/metroid/metroid.xml"].onload();
          DataRequest.prototype.requests["/themes/metroid/fonts/metroid.ttf"].onload();
          DataRequest.prototype.requests["/themes/metroid/images/metroid.png"].onload();
          DataRequest.prototype.requests["/themes/metroid/images/metroid.xml"].onload();
          DataRequest.prototype.requests["/themes/minimal/minimal.xml"].onload();
          DataRequest.prototype.requests["/themes/minimal/fonts/pf_ronda_seven.ttf"].onload();
          DataRequest.prototype.requests["/themes/minimal/fonts/pf_ronda_seven_bold.ttf"].onload();
          DataRequest.prototype.requests["/themes/minimal/images/minimal.png"].onload();
          DataRequest.prototype.requests["/themes/minimal/images/minimal.xml"].onload();
          DataRequest.prototype.requests["/gamedata-sample/animation_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/dialog_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/game_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/gui_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/helloworld_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/io_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/main.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/menu_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/music_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/particle_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/physics_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/platform.tmx"].onload();
          DataRequest.prototype.requests["/gamedata-sample/platform_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/primitive_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/project.rap"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sound_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sprite_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/textbox_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/tween_scene.rsc"].onload();
          DataRequest.prototype.requests["/gamedata-sample/fonts/casual.ttf"].onload();
          DataRequest.prototype.requests["/gamedata-sample/fonts/consola.ttf"].onload();
          DataRequest.prototype.requests["/gamedata-sample/fonts/kinkie.ttf"].onload();
          DataRequest.prototype.requests["/gamedata-sample/fonts/vitamin.ttf"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/apple-bg.jpg"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/back1.jpg"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/broute.png"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/broute.xml"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/level1.tmx"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/platform1.bmp"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/Sheep.png"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/sheet.png"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/sheet.xml"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/tiles_spritesheet.png"].onload();
          DataRequest.prototype.requests["/gamedata-sample/graphics/tiles_spritesheet.xml"].onload();
          DataRequest.prototype.requests["/gamedata-sample/particles/snow.xml"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sounds/invitation.mod"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sounds/pop.ogg"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sounds/ressort.ogg"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sounds/sample.ogg"].onload();
          DataRequest.prototype.requests["/gamedata-sample/sounds/sheep.ogg"].onload();
          DataRequest.prototype.requests["/gamedata-sample/strings/en.xml"].onload();
          DataRequest.prototype.requests["/gamedata-sample/strings/fr.xml"].onload();
          Module['removeRunDependency']('datafile_trp.data');

    };
    Module['addRunDependency']('datafile_trp.data');
  
    if (!Module.preloadResults) Module.preloadResults = {};
  
      Module.preloadResults[PACKAGE_NAME] = {fromCache: false};
      if (fetched) {
        processPackageData(fetched);
        fetched = null;
      } else {
        fetchedCallback = processPackageData;
      }
    
  }
  if (Module['calledRun']) {
    runWithFS();
  } else {
    if (!Module['preRun']) Module['preRun'] = [];
    Module["preRun"].push(runWithFS); // FS is not initialized yet, wait for it
  }

 }
 loadPackage();

})();
