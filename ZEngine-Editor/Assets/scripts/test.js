function TestComponent()
{
	this.delta = 0.0;
}

TestComponent.prototype.Init = function()
{
	// Called on start
	this.owner.transform.SetScale(1.5, 1.5, 1.5);
	
	var testVec = new vec3(0, 10, 15);
	
	testVec.x = 10;
	
	log("x: " + testVec.x);
	log("y: " + testVec.y);
	log("z: " + testVec.z);	
	
	log("called for object: " + this.owner.name);
};
	
TestComponent.prototype.Update = function()
{
	// Called once per frame
	
}	

TestComponent.prototype.Render = function()
{
	// Called once per frame after update
	
	var position = this.owner.transform.GetPosition();
	var transform = this.owner.transform;
	
	/*if (Input.GetButtonUp(BUTTON_KEY_A))
	{
		var map = MapManager.currentMap;
		var object = map.Find("Cube Object");
				
		if (object != null)
		{
			var clone = Factory.Copy("Clone Object", object);
			clone.transform.position = new vec3(-5 + Math.random() * 10, -5 + Math.random() * 10, 0);
			
			log("just created a clone called: " + clone.name);
			
			map.Add(clone);
		}
		else
		{
			log("Failed to find the object!");
		}
	}*/
	
	if (Input.GetButtonDown(BUTTON_KEY_RIGHT))
	{		
		transform.SetPosition(position.x + 2 * Time.delta, position.y, position.z);
	}
	else if (Input.GetButtonDown(BUTTON_KEY_LEFT))
	{		
		transform.SetPosition(position.x - 2 * Time.delta, position.y, position.z);
	}
	
	if (Input.GetButtonDown(BUTTON_KEY_UP))
	{		
		transform.SetPosition(position.x, position.y - 2 * Time.delta, position.z);
	}
	else if (Input.GetButtonDown(BUTTON_KEY_DOWN))
	{		
		transform.SetPosition(position.x, position.y + 2 * Time.delta, position.z);
	}
	
	this.delta += 0.01;
};
	
TestComponent.prototype.AdditionalFunction = function()
{
	// TODO: Do stuff extra here
};