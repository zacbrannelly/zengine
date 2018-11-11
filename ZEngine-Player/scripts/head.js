function HeadComponent()
{
	this.speed = 2;
	this.velocity = new vec3(this.speed, 0, 0);
	this.boxCollider = null;
	this.food = null;
	this.body = [];
}

HeadComponent.prototype.Init = function()
{
	this.boxCollider = this.owner.GetComponent("BoxCollider2D");
	this.boxCollider.width = 0.4;
	this.boxCollider.height = 0.4;
	
	this.food = MapManager.currentMap.Find("Food").GetComponent("FoodComponent");
}

HeadComponent.prototype.Update = function()
{
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
	
	var position = this.owner.transform.position;
	this.owner.transform.position = new vec3(position.x + this.velocity.x * Time.delta, position.y + this.velocity.y * Time.delta, position.y + this.velocity.z * Time.delta);
	
	if (this.food.boxCollider.Intersects(this.boxCollider))
	{
		this.food.Eat();
	}
	
	if (Input.GetButtonUp(BUTTON_KEY_T))
	{
		var copy = Factory.Copy(this.owner);
		
		// Remove the head component
		var headComp = copy.GetComponent("HeadComponent");
		headComp.RemoveComponent(headComp);
		
		MapManager.currentMap.Add(copy);
	}
}

HeadComponent.prototype.Render = function()
{
	
}