function RotateComponent()
{
	this.delta = 0;
}

RotateComponent.prototype.Init = function()
{
	
}

RotateComponent.prototype.Update = function()
{
	
}

RotateComponent.prototype.Render = function()
{
	this.owner.transform.SetRotation(0, this.delta, 0);
	this.delta += 0.01;
}