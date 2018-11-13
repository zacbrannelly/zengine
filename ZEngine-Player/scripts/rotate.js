function RotateComponent()
{
	this.delta = 0.0;
}

RotateComponent.prototype.Init = function()
{
}

RotateComponent.prototype.Update = function()
{
	this.owner.transform.SetRotation(this.delta, this.delta, 0);
	this.delta += 90 * Time.delta;	
}

RotateComponent.prototype.Render = function()
{
}