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
        "String": 10,
        "Foo": 100,
        "Emacs": 1,
        "Car": 10,
        "Bar": 10,
        "Baz": 20,
        "User": 5,
        "Post": 10,
        "Comment": 100,
        "Website": 10,
        "Array": 500
      };

      counter.addToCurrentObjects(currentObject);
      expect(counter.currentObjectCount["Array"]).toBe(500);
      var timeSeries = counter.timeSeries();
      expect(timeSeries["Array"]).toBe(500);
      expect(_.keys(timeSeries).length).toBe(9);
    });
  });
});
