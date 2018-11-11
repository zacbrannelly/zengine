function FoodComponent()
{
	this.boxCollider = null;
}

FoodComponent.prototype.Init = function()
{
	this.boxCollider = this.owner.GetComponent("BoxCollider2D");	
	this.boxCollider.width = 0.4;
	this.boxCollider.height = 0.4;
}

FoodComponent.prototype.Eat = function()
{
	this.owner.transform.position = new vec3(-5 + 10 * Math.random(), -5 + 10 * Math.random(), 0);
	// TODO: Reward the player
}

FoodComponent.prototype.Update = function()
{

}

FoodComponent.prototype.Render = function()
{
	
}