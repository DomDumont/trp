
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
  
    var REMOTE_PACKAGE_SIZE = metadata.remote_package_size;
    var PACKAGE_UUID = metadata.package_uuid;
  
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

        var files = metadata.files;
        for (i = 0; i < files.length; ++i) {
          new DataRequest(files[i].start, files[i].end, files[i].crunched, files[i].audio).open('GET', files[i].filename);
        }

  
    function processPackageData(arrayBuffer) {
      Module.finishedDataFileDownloads++;
      assert(arrayBuffer, 'Loading data file failed.');
      assert(arrayBuffer instanceof ArrayBuffer, 'bad input to processPackageData');
      var byteArray = new Uint8Array(arrayBuffer);
      var curr;
      
        // copy the entire loaded file into a spot in the heap. Files will refer to slices in that. They cannot be freed though
        // (we may be allocating before malloc is ready, during startup).
        var ptr = Module['getMemory'](byteArray.length);
        Module['HEAPU8'].set(byteArray, ptr);
        DataRequest.prototype.byteArray = Module['HEAPU8'].subarray(ptr, ptr+byteArray.length);
  
          var files = metadata.files;
          for (i = 0; i < files.length; ++i) {
            DataRequest.prototype.requests[files[i].filename].onload();
          }
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
 loadPackage({"files": [{"audio": "0", "start": 0, "crunched": "0", "end": 518, "filename": "/settings.xml"}, {"audio": "0", "start": 518, "crunched": "0", "end": 2180, "filename": "/helpers/scene_manager.rsc"}, {"audio": "0", "start": 2180, "crunched": "0", "end": 3082, "filename": "/helpers/trp_api.rsc"}, {"audio": "0", "start": 3082, "crunched": "0", "end": 4045, "filename": "/themes/aeon/aeon.xml"}, {"audio": "0", "start": 4045, "crunched": "0", "end": 45065, "filename": "/themes/aeon/fonts/aeon_main.ttf"}, {"audio": "0", "start": 45065, "crunched": "0", "end": 125329, "filename": "/themes/aeon/fonts/pf_ronda_seven.ttf"}, {"audio": "0", "start": 125329, "crunched": "0", "end": 198193, "filename": "/themes/aeon/fonts/pf_ronda_seven_bold.ttf"}, {"audio": "0", "start": 198193, "crunched": "0", "end": 215440, "filename": "/themes/aeon/images/aeon.png"}, {"audio": "0", "start": 215440, "crunched": "0", "end": 224448, "filename": "/themes/aeon/images/aeon.xml"}, {"audio": "0", "start": 224448, "crunched": "0", "end": 225351, "filename": "/themes/castlevania/castlevania.xml"}, {"audio": "0", "start": 225351, "crunched": "0", "end": 290027, "filename": "/themes/castlevania/fonts/lohengrin.ttf"}, {"audio": "0", "start": 290027, "crunched": "0", "end": 340115, "filename": "/themes/castlevania/images/castlevania.png"}, {"audio": "0", "start": 340115, "crunched": "0", "end": 345905, "filename": "/themes/castlevania/images/castlevania.xml"}, {"audio": "0", "start": 345905, "crunched": "0", "end": 346885, "filename": "/themes/metal/metal.xml"}, {"audio": "0", "start": 346885, "crunched": "0", "end": 496393, "filename": "/themes/metal/fonts/SourceSansPro-Regular.ttf"}, {"audio": "0", "start": 496393, "crunched": "0", "end": 645745, "filename": "/themes/metal/fonts/SourceSansPro-Semibold.ttf"}, {"audio": "0", "start": 645745, "crunched": "0", "end": 680554, "filename": "/themes/metal/images/metal.png"}, {"audio": "0", "start": 680554, "crunched": "0", "end": 685508, "filename": "/themes/metal/images/metal.xml"}, {"audio": "0", "start": 685508, "crunched": "0", "end": 686405, "filename": "/themes/metroid/metroid.xml"}, {"audio": "0", "start": 686405, "crunched": "0", "end": 695261, "filename": "/themes/metroid/fonts/metroid.ttf"}, {"audio": "0", "start": 695261, "crunched": "0", "end": 722775, "filename": "/themes/metroid/images/metroid.png"}, {"audio": "0", "start": 722775, "crunched": "0", "end": 728558, "filename": "/themes/metroid/images/metroid.xml"}, {"audio": "0", "start": 728558, "crunched": "0", "end": 729462, "filename": "/themes/minimal/minimal.xml"}, {"audio": "0", "start": 729462, "crunched": "0", "end": 809726, "filename": "/themes/minimal/fonts/pf_ronda_seven.ttf"}, {"audio": "0", "start": 809726, "crunched": "0", "end": 882590, "filename": "/themes/minimal/fonts/pf_ronda_seven_bold.ttf"}, {"audio": "0", "start": 882590, "crunched": "0", "end": 909408, "filename": "/themes/minimal/images/minimal.png"}, {"audio": "0", "start": 909408, "crunched": "0", "end": 914441, "filename": "/themes/minimal/images/minimal.xml"}, {"audio": "0", "start": 914441, "crunched": "0", "end": 915923, "filename": "/gamedata-sample/animation_scene.rsc"}, {"audio": "0", "start": 915923, "crunched": "0", "end": 917502, "filename": "/gamedata-sample/dialog_scene.rsc"}, {"audio": "0", "start": 917502, "crunched": "0", "end": 921401, "filename": "/gamedata-sample/game_scene.rsc"}, {"audio": "0", "start": 921401, "crunched": "0", "end": 923664, "filename": "/gamedata-sample/gui_scene.rsc"}, {"audio": "0", "start": 923664, "crunched": "0", "end": 925213, "filename": "/gamedata-sample/helloworld_scene.rsc"}, {"audio": "0", "start": 925213, "crunched": "0", "end": 927444, "filename": "/gamedata-sample/io_scene.rsc"}, {"audio": "0", "start": 927444, "crunched": "0", "end": 931180, "filename": "/gamedata-sample/main.rsc"}, {"audio": "0", "start": 931180, "crunched": "0", "end": 936552, "filename": "/gamedata-sample/menu_scene.rsc"}, {"audio": "0", "start": 936552, "crunched": "0", "end": 937866, "filename": "/gamedata-sample/music_scene.rsc"}, {"audio": "0", "start": 937866, "crunched": "0", "end": 939575, "filename": "/gamedata-sample/particle_scene.rsc"}, {"audio": "0", "start": 939575, "crunched": "0", "end": 943952, "filename": "/gamedata-sample/physics_scene.rsc"}, {"audio": "0", "start": 943952, "crunched": "0", "end": 953580, "filename": "/gamedata-sample/platform.tmx"}, {"audio": "0", "start": 953580, "crunched": "0", "end": 955151, "filename": "/gamedata-sample/platform_scene.rsc"}, {"audio": "0", "start": 955151, "crunched": "0", "end": 956962, "filename": "/gamedata-sample/primitive_scene.rsc"}, {"audio": "0", "start": 956962, "crunched": "0", "end": 957589, "filename": "/gamedata-sample/project.rap"}, {"audio": "0", "start": 957589, "crunched": "0", "end": 958813, "filename": "/gamedata-sample/sound_scene.rsc"}, {"audio": "0", "start": 958813, "crunched": "0", "end": 960543, "filename": "/gamedata-sample/sprite_scene.rsc"}, {"audio": "0", "start": 960543, "crunched": "0", "end": 962294, "filename": "/gamedata-sample/textbox_scene.rsc"}, {"audio": "0", "start": 962294, "crunched": "0", "end": 964062, "filename": "/gamedata-sample/tween_scene.rsc"}, {"audio": "0", "start": 964062, "crunched": "0", "end": 988966, "filename": "/gamedata-sample/fonts/casual.ttf"}, {"audio": "0", "start": 988966, "crunched": "0", "end": 1347222, "filename": "/gamedata-sample/fonts/consola.ttf"}, {"audio": "0", "start": 1347222, "crunched": "0", "end": 1440686, "filename": "/gamedata-sample/fonts/kinkie.ttf"}, {"audio": "0", "start": 1440686, "crunched": "0", "end": 1462430, "filename": "/gamedata-sample/fonts/vitamin.ttf"}, {"audio": "0", "start": 1462430, "crunched": "0", "end": 2043228, "filename": "/gamedata-sample/graphics/apple-bg.jpg"}, {"audio": "0", "start": 2043228, "crunched": "0", "end": 2094962, "filename": "/gamedata-sample/graphics/back1.jpg"}, {"audio": "0", "start": 2094962, "crunched": "0", "end": 2626182, "filename": "/gamedata-sample/graphics/broute.png"}, {"audio": "0", "start": 2626182, "crunched": "0", "end": 2633077, "filename": "/gamedata-sample/graphics/broute.xml"}, {"audio": "0", "start": 2633077, "crunched": "0", "end": 2642056, "filename": "/gamedata-sample/graphics/level1.tmx"}, {"audio": "0", "start": 2642056, "crunched": "0", "end": 2704426, "filename": "/gamedata-sample/graphics/platform1.bmp"}, {"audio": "0", "start": 2704426, "crunched": "0", "end": 2720278, "filename": "/gamedata-sample/graphics/Sheep.png"}, {"audio": "0", "start": 2720278, "crunched": "0", "end": 2744501, "filename": "/gamedata-sample/graphics/sheet.png"}, {"audio": "0", "start": 2744501, "crunched": "0", "end": 2744911, "filename": "/gamedata-sample/graphics/sheet.xml"}, {"audio": "0", "start": 2744911, "crunched": "0", "end": 2873746, "filename": "/gamedata-sample/graphics/tiles_spritesheet.png"}, {"audio": "0", "start": 2873746, "crunched": "0", "end": 2887311, "filename": "/gamedata-sample/graphics/tiles_spritesheet.xml"}, {"audio": "0", "start": 2887311, "crunched": "0", "end": 2887412, "filename": "/gamedata-sample/particles/snow.xml"}, {"audio": "0", "start": 2887412, "crunched": "0", "end": 3027290, "filename": "/gamedata-sample/sounds/invitation.mod"}, {"audio": "1", "start": 3027290, "crunched": "0", "end": 3049835, "filename": "/gamedata-sample/sounds/pop.ogg"}, {"audio": "1", "start": 3049835, "crunched": "0", "end": 3068261, "filename": "/gamedata-sample/sounds/ressort.ogg"}, {"audio": "1", "start": 3068261, "crunched": "0", "end": 3350033, "filename": "/gamedata-sample/sounds/sample.ogg"}, {"audio": "1", "start": 3350033, "crunched": "0", "end": 3370344, "filename": "/gamedata-sample/sounds/sheep.ogg"}, {"audio": "0", "start": 3370344, "crunched": "0", "end": 3370498, "filename": "/gamedata-sample/strings/en.xml"}, {"audio": "0", "start": 3370498, "crunched": "0", "end": 3370653, "filename": "/gamedata-sample/strings/fr.xml"}], "remote_package_size": 3370653, "package_uuid": "20c45d59-363c-4264-bc68-266f01aad94e"});

})();
