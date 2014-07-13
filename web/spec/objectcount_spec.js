describe("Object Coung spec", function() {
  describe("Adding data to current Objects", function() {
    var counter = new ObjectCount();
    afterEach(function() {
      counter.resetObjectCount();
    });

    it("should populate current Objects", function() {
      counter.addToCurrentObjects({"String": 10, "Foo": 100});
      expect(counter.currentObjectCount["String"]).toBe(10);
      expect(counter.currentObjectCount["Foo"]).toBe(100);
      var timeSeries = counter.timeSeries();
      expect(timeSeries["String"]).toBe(10);
      expect(timeSeries["Foo"]).toBe(100);
    });

    it("should popultae Other Objects if knownClasses is more than 8", function() {
      var currentObject = {
        "String": 3,
        "Foo": 100,
        "Emacs": 5,
        "Car": 6,
        "Bar": 8,
        "Baz": 11,
        "User": 12,
        "Post": 13,
        "Comment": 14,
        "Website": 15,
        "Array": 500
      };

      counter.addToCurrentObjects(currentObject);
      expect(counter.currentObjectCount["Array"]).toBe(500);
      expect(counter.currentObjectCount["String"]).toBe(3);
      var timeSeries = counter.timeSeries();
      expect(timeSeries["Array"]).toBe(500);
      expect(timeSeries["Other"]).toBe(14);
      expect(_.keys(timeSeries).length).toBe(9);
    });
  });
});
