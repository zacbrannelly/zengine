function HeadComponent()
{
	this.speed = 4;
	this.velocity = new vec3(this.speed, 0, 0);
	this.boxCollider = null;
	this.food = null;
	this.body = [];
	
	// Create once (avoiding GC)
	this.position = new vec3(0, 0, 0);
}

HeadComponent.prototype.Init = function()
{
	this.boxCollider = this.owner.GetComponent("BoxCollider2D");
	this.boxCollider.width = this.owner.transform.scale.x * 2;
	this.boxCollider.height = this.owner.transform.scale.y * 2;
	
	this.body.push(this.owner);
	
	this.food = MapManager.currentMap.Find("Food").GetComponent("FoodComponent");
}

HeadComponent.prototype.Update = function()
{
	// Move according to input
	if (Input.GetButtonDown(BUTTON_KEY_A))
	{
		this.velocity.x = -this.speed;
		this.velocity.y = 0;
	}
	else if (Input.GetButtonDown(BUTTON_KEY_D))
	{
		this.velocity.x = this.speed;
		this.velocity.y = 0;
	}
	
	if (Input.GetButtonDown(BUTTON_KEY_W))
	{
		this.velocity.x = 0;
		this.velocity.y = this.speed;
	}
	else if (Input.GetButtonDown(BUTTON_KEY_S))
	{
		this.velocity.x = 0;
		this.velocity.y = -this.speed;
	}
	
	// Update the position of the object using velocity
	var position = this.owner.transform.GetPosition(this.position);
	this.owner.transform.SetPosition(position.x + this.velocity.x * Time.delta, position.y + this.velocity.y * Time.delta, position.y + this.velocity.z * Time.delta);
	
	// Check if we have eaten the food
	if (this.food.boxCollider.Intersects(this.boxCollider))
	{
		this.food.Eat();
		this.SpawnBody();
	}
}

HeadComponent.prototype.SpawnBody = function()
{
	// Copy the last body object in the body list
	var source = this.body[this.body.length - 1];
	var copy = Factory.Copy("Body " + this.body.length, this.body[this.body.length - 1]);
				
	// Remove the head component, if any (first clone is the head)
	var headComp = copy.GetComponent("HeadComponent");
	if (headComp != null)
		copy.RemoveComponent(headComp);
		
	// Get the body components from the source and copy
	var sourceBodyComp = source.GetComponent("BodyComponent");
	var bodyComponent = copy.GetComponent("BodyComponent");
	
	// Add the new body part to the map and body array
	MapManager.currentMap.Add(copy);
	this.body.push(copy);
		
	// Link the new copy to the source body component (so it follows the source obj)
	sourceBodyComp.child = copy;
}

HeadComponent.prototype.Render = function()
{
	
}