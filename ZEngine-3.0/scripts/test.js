// This is the constructor, create all fields in here
function TestComponent()
{
	this.testVariable = "helllll yeah";
	this.delta = 0.0;
};

TestComponent.prototype.Init = function()
{
	// Called on start
	log("This is a test log from a script!");
};
	
TestComponent.prototype.Update = function()
{
	// Called once per frame
};	

TestComponent.prototype.Render = function()
{
	// Called once per frame after update
	this.entity.transform.SetPosition(5.0 * Math.sin(this.delta),  5.0 * Math.cos(this.delta), -5);
	this.entity.transform.SetRotation(0, this.delta * 100, 0);
	this.entity.transform.SetScale(Math.cos(this.delta / 2), Math.sin(this.delta / 2), Math.sin(this.delta / 2));
	
	this.delta += 0.01;
};
	
TestComponent.prototype.AdditionalFunction = function()
{
	// TODO: Do stuff extra here
};