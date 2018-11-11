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
	
	var testVec = new vec3(0, 10, 15);
	
	testVec.x = 10;
	
	log("x: " + testVec.x);
	log("y: " + testVec.y);
	log("z: " + testVec.z);
};
	
TestComponent.prototype.Update = function()
{
	// Called once per frame
	
};	

TestComponent.prototype.Render = function()
{
	// Called once per frame after update
	//this.owner.transform.SetPosition(5.0 * Math.sin(this.delta),  5.0 * Math.cos(this.delta), -5);
	//this.owner.transform.SetRotation(0, this.delta * 100, 0);
	//this.owner.transform.SetScale(1.5 * Math.cos(this.delta / 2), 1.5 * Math.sin(this.delta / 2), 1.5 * Math.sin(this.delta / 2));
		
	var position = this.owner.transform.GetPosition();
	
	if (Input.GetButtonDown(BUTTON_KEY_RIGHT))
	{		
		log("doing this");

		this.owner.transform.SetPosition(position.x + 2 * Time.delta, position.y, position.z);
	}
	else if (Input.GetButtonDown(BUTTON_KEY_LEFT))
	{		
		this.owner.transform.SetPosition(position.x - 2 * Time.delta, position.y, position.z);
	}
	
	if (Input.GetButtonDown(BUTTON_KEY_UP))
	{		
		this.owner.transform.SetPosition(position.x, position.y - 2 * Time.delta, position.z);
	}
	else if (Input.GetButtonDown(BUTTON_KEY_DOWN))
	{		
		this.owner.transform.SetPosition(position.x, position.y + 2 * Time.delta, position.z);
	}
	
	this.delta += 0.01;	
};
	
TestComponent.prototype.AdditionalFunction = function()
{
	// TODO: Do stuff extra here
};