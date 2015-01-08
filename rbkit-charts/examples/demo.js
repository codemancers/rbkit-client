// Underscore random function

var _ = {
  random: function(max, min) {
    if (max == null) {
      max = min;
      min = 0;
    }
    return min + Math.floor(Math.random() * (max - min + 1));
  }
}
// ============================== code for heap charts =============================

var heapRandomData = function () {
  return {
    'Heap Objects': _.random(0, 1000),
    'Heap Size':    _.random(0, 200),
    'Mem Size':     _.random(0, 200)
  };
};

var heapTimeout;
var heapCharts = function() {
  heapTimeout = setTimeout(heapCharts, 1000);

  Rbkit.updateLiveObjectsChart(heapRandomData());
  Rbkit.updateHeapChart(heapRandomData());
};

heapCharts();


// =============================== code for gc charts ==============================
// assuming max gc time is 10s

var gcStartTimeout;
var gcChartStart = function () {
  gcStartTimeout = setTimeout(gcChartEnd, _.random(0, 10) * 1000);

  Rbkit.gcStarted(new Date());
};

var gcEndTimeout;
var gcChartEnd = function () {
  gcEndTimeout = setTimeout(gcChartStart, 5000);

  Rbkit.gcEnded(new Date());
};


gcChartStart();

// ==================== code for randomly updating polar charts ====================
var polarRandomDataIter = 0;
var polarRandomData = function () {
  var randomLimit = (polarRandomDataIter++ % 2 == 0) ? 200 : 100;

  return {
    String: _.random(0, randomLimit),
    Foo:    _.random(0, randomLimit),
    Bar:    _.random(0, randomLimit),
    Others: _.random(0, randomLimit)
  };
};

var polarGenerationCharts = function () {
  setTimeout(polarGenerationCharts, 5000);

  Rbkit.updateYoungGenerationChart(polarRandomData());
  Rbkit.updateSecondGenerationChart(polarRandomData());
  Rbkit.updateOldGenerationChart(polarRandomData());
};

// polarGenerationCharts();

// ====================== code for randomly updating gc stats ======================

// => {:count=>6, :heap_used=>81, :heap_length=>81, :heap_increment=>0,
// :heap_live_slot=>32490, :heap_free_slot=>525, :heap_final_slot=>0,
// :heap_swept_slot=>16954, :heap_eden_page_length=>81, :heap_tomb_page_length=>0,
// :total_allocated_object=>78454, :total_freed_object=>45964,
// :malloc_increase=>799520, :malloc_limit=>16777216, :minor_gc_count=>4,
// :major_gc_count=>2, :remembered_shady_object=>170,
// :remembered_shady_object_limit=>336, :old_object=>8736, :old_object_limit=>17472,
// :oldmalloc_increase=>799904, :oldmalloc_limit=>16777216}

var gcRandomStats = function () {
  return {
    count:          _.random(0, 100),
    heap_used:      _.random(0, 100),
    heap_length:    _.random(0, 100),
    heap_increment: _.random(0, 10),
    heap_live_slot: _.random(0, 100000),
    heap_free_slot: _.random(0, 1000),
    heap_final_slot: _.random(0, 100),
    heap_swept_slot: _.random(0, 100000),
    heap_eden_page_length: _.random(0, 100),
    heap_tomb_page_length: _.random(0, 100),
    total_allocated_object: _.random(0, 100000),
    total_freed_object: _.random(0, 100000),
    malloc_increase: _.random(0, 1000000),
    malloc_limit: _.random(0, 20000000),
    minor_gc_count: _.random(0, 100),
    major_gc_count: _.random(0, 100),
    remembered_shady_object: _.random(0, 1000),
    remembered_shady_object_limit: _.random(0, 1000),
    old_object: _.random(0, 10000),
    old_object_limit: _.random(0, 100000),
    oldmalloc_increase: _.random(0, 1000000),
    oldmalloc_limit: _.random(0, 20000000)
  };
};

var gcStatsTimeout;
var gcStatsTable = function () {
  gcStatsTimeout = setTimeout(gcStatsTable, 5000);

  Rbkit.updateGcStats(gcRandomStats());
};

gcStatsTable();

var reset = function() {
  clearTimeout(heapTimeout);
  clearTimeout(gcEndTimeout);
  clearTimeout(gcStartTimeout);
  clearTimeout(gcStatsTimeout);

  Rbkit.reset();

  setTimeout(function() {
    heapCharts();
    gcChartStart();
    gcStatsTable();
  }, 2000);
};

document.getElementById("reset").addEventListener("click", reset);
