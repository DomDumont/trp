
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
  
      var REMOTE_PACKAGE_SIZE = 3368051;
      var PACKAGE_UUID = '442087df-d6ab-4be1-9903-fff204ad8fc6';
    
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
    new DataRequest(518, 1481, 0, 0).open('GET', '/themes/aeon/aeon.xml');
    new DataRequest(1481, 42501, 0, 0).open('GET', '/themes/aeon/fonts/aeon_main.ttf');
    new DataRequest(42501, 122765, 0, 0).open('GET', '/themes/aeon/fonts/pf_ronda_seven.ttf');
    new DataRequest(122765, 195629, 0, 0).open('GET', '/themes/aeon/fonts/pf_ronda_seven_bold.ttf');
    new DataRequest(195629, 212876, 0, 0).open('GET', '/themes/aeon/images/aeon.png');
    new DataRequest(212876, 221884, 0, 0).open('GET', '/themes/aeon/images/aeon.xml');
    new DataRequest(221884, 222787, 0, 0).open('GET', '/themes/castlevania/castlevania.xml');
    new DataRequest(222787, 287463, 0, 0).open('GET', '/themes/castlevania/fonts/lohengrin.ttf');
    new DataRequest(287463, 337551, 0, 0).open('GET', '/themes/castlevania/images/castlevania.png');
    new DataRequest(337551, 343341, 0, 0).open('GET', '/themes/castlevania/images/castlevania.xml');
    new DataRequest(343341, 344321, 0, 0).open('GET', '/themes/metal/metal.xml');
    new DataRequest(344321, 493829, 0, 0).open('GET', '/themes/metal/fonts/SourceSansPro-Regular.ttf');
    new DataRequest(493829, 643181, 0, 0).open('GET', '/themes/metal/fonts/SourceSansPro-Semibold.ttf');
    new DataRequest(643181, 677990, 0, 0).open('GET', '/themes/metal/images/metal.png');
    new DataRequest(677990, 682944, 0, 0).open('GET', '/themes/metal/images/metal.xml');
    new DataRequest(682944, 683841, 0, 0).open('GET', '/themes/metroid/metroid.xml');
    new DataRequest(683841, 692697, 0, 0).open('GET', '/themes/metroid/fonts/metroid.ttf');
    new DataRequest(692697, 720211, 0, 0).open('GET', '/themes/metroid/images/metroid.png');
    new DataRequest(720211, 725994, 0, 0).open('GET', '/themes/metroid/images/metroid.xml');
    new DataRequest(725994, 726898, 0, 0).open('GET', '/themes/minimal/minimal.xml');
    new DataRequest(726898, 807162, 0, 0).open('GET', '/themes/minimal/fonts/pf_ronda_seven.ttf');
    new DataRequest(807162, 880026, 0, 0).open('GET', '/themes/minimal/fonts/pf_ronda_seven_bold.ttf');
    new DataRequest(880026, 906844, 0, 0).open('GET', '/themes/minimal/images/minimal.png');
    new DataRequest(906844, 911877, 0, 0).open('GET', '/themes/minimal/images/minimal.xml');
    new DataRequest(911877, 913359, 0, 0).open('GET', '/gamedata-sample/animation_scene.rsc');
    new DataRequest(913359, 914938, 0, 0).open('GET', '/gamedata-sample/dialog_scene.rsc');
    new DataRequest(914938, 918837, 0, 0).open('GET', '/gamedata-sample/game_scene.rsc');
    new DataRequest(918837, 921100, 0, 0).open('GET', '/gamedata-sample/gui_scene.rsc');
    new DataRequest(921100, 922649, 0, 0).open('GET', '/gamedata-sample/helloworld_scene.rsc');
    new DataRequest(922649, 924880, 0, 0).open('GET', '/gamedata-sample/io_scene.rsc');
    new DataRequest(924880, 928616, 0, 0).open('GET', '/gamedata-sample/main.rsc');
    new DataRequest(928616, 933988, 0, 0).open('GET', '/gamedata-sample/menu_scene.rsc');
    new DataRequest(933988, 935302, 0, 0).open('GET', '/gamedata-sample/music_scene.rsc');
    new DataRequest(935302, 937011, 0, 0).open('GET', '/gamedata-sample/particle_scene.rsc');
    new DataRequest(937011, 941350, 0, 0).open('GET', '/gamedata-sample/physics_scene.rsc');
    new DataRequest(941350, 950978, 0, 0).open('GET', '/gamedata-sample/platform.tmx');
    new DataRequest(950978, 952549, 0, 0).open('GET', '/gamedata-sample/platform_scene.rsc');
    new DataRequest(952549, 954360, 0, 0).open('GET', '/gamedata-sample/primitive_scene.rsc');
    new DataRequest(954360, 954987, 0, 0).open('GET', '/gamedata-sample/project.rap');
    new DataRequest(954987, 956211, 0, 0).open('GET', '/gamedata-sample/sound_scene.rsc');
    new DataRequest(956211, 957941, 0, 0).open('GET', '/gamedata-sample/sprite_scene.rsc');
    new DataRequest(957941, 959692, 0, 0).open('GET', '/gamedata-sample/textbox_scene.rsc');
    new DataRequest(959692, 961460, 0, 0).open('GET', '/gamedata-sample/tween_scene.rsc');
    new DataRequest(961460, 986364, 0, 0).open('GET', '/gamedata-sample/fonts/casual.ttf');
    new DataRequest(986364, 1344620, 0, 0).open('GET', '/gamedata-sample/fonts/consola.ttf');
    new DataRequest(1344620, 1438084, 0, 0).open('GET', '/gamedata-sample/fonts/kinkie.ttf');
    new DataRequest(1438084, 1459828, 0, 0).open('GET', '/gamedata-sample/fonts/vitamin.ttf');
    new DataRequest(1459828, 2040626, 0, 0).open('GET', '/gamedata-sample/graphics/apple-bg.jpg');
    new DataRequest(2040626, 2092360, 0, 0).open('GET', '/gamedata-sample/graphics/back1.jpg');
    new DataRequest(2092360, 2623580, 0, 0).open('GET', '/gamedata-sample/graphics/broute.png');
    new DataRequest(2623580, 2630475, 0, 0).open('GET', '/gamedata-sample/graphics/broute.xml');
    new DataRequest(2630475, 2639454, 0, 0).open('GET', '/gamedata-sample/graphics/level1.tmx');
    new DataRequest(2639454, 2701824, 0, 0).open('GET', '/gamedata-sample/graphics/platform1.bmp');
    new DataRequest(2701824, 2717676, 0, 0).open('GET', '/gamedata-sample/graphics/Sheep.png');
    new DataRequest(2717676, 2741899, 0, 0).open('GET', '/gamedata-sample/graphics/sheet.png');
    new DataRequest(2741899, 2742309, 0, 0).open('GET', '/gamedata-sample/graphics/sheet.xml');
    new DataRequest(2742309, 2871144, 0, 0).open('GET', '/gamedata-sample/graphics/tiles_spritesheet.png');
    new DataRequest(2871144, 2884709, 0, 0).open('GET', '/gamedata-sample/graphics/tiles_spritesheet.xml');
    new DataRequest(2884709, 2884810, 0, 0).open('GET', '/gamedata-sample/particles/snow.xml');
    new DataRequest(2884810, 3024688, 0, 0).open('GET', '/gamedata-sample/sounds/invitation.mod');
    new DataRequest(3024688, 3047233, 0, 1).open('GET', '/gamedata-sample/sounds/pop.ogg');
    new DataRequest(3047233, 3065659, 0, 1).open('GET', '/gamedata-sample/sounds/ressort.ogg');
    new DataRequest(3065659, 3347431, 0, 1).open('GET', '/gamedata-sample/sounds/sample.ogg');
    new DataRequest(3347431, 3367742, 0, 1).open('GET', '/gamedata-sample/sounds/sheep.ogg');
    new DataRequest(3367742, 3367896, 0, 0).open('GET', '/gamedata-sample/strings/en.xml');
    new DataRequest(3367896, 3368051, 0, 0).open('GET', '/gamedata-sample/strings/fr.xml');

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
