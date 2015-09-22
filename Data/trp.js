
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
  
      var REMOTE_PACKAGE_SIZE = 914441;
      var PACKAGE_UUID = 'dbfe24a3-78c5-4b20-b872-b751303114b3';
    
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
Module['FS_createPath']('/', 'helpers', true, true);

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
    new DataRequest(911877, 913539, 0, 0).open('GET', '/helpers/scene_manager.rsc');
    new DataRequest(913539, 914441, 0, 0).open('GET', '/helpers/trp_api.rsc');

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
          DataRequest.prototype.requests["/helpers/scene_manager.rsc"].onload();
          DataRequest.prototype.requests["/helpers/trp_api.rsc"].onload();
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
