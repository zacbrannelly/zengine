// This is the constructor, create all fields in here
function TestComponent()
{
	Component.call(this)
	
	this.testVariable = "helllll yeah";
	this.delta = 0.0;
};

TestComponent.prototype = new Component();

TestComponent.prototype.Init = function()
{
	// Called on start
	log("This is a test log from a script!");	
	
	this.owner.transform.SetScale(1.5, 1.5, 1.5);
};
	
TestComponent.prototype.Update = function()
{
	// Called once per frame
};	

TestComponent.prototype.Render = function()
{
	// Called once per frame after update
	this.owner.transform.SetPosition(5.0 * Math.sin(this.delta),  5.0 * Math.cos(this.delta), -5);
	this.owner.transform.SetRotation(0, this.delta * 100, 0);
	//this.owner.transform.SetScale(1.5 * Math.cos(this.delta / 2), 1.5 * Math.sin(this.delta / 2), 1.5 * Math.sin(this.delta / 2));
	
	this.delta += 0.01;
};
	
TestComponent.prototype.AdditionalFunction = function()
{
	// TODO: Do stuff extra here
};