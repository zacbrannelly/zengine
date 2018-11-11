function BoxCollider2D()
{
	this.position = new vec2(0, 0);
	this.width = 1;
	this.height = 1;
}

BoxCollider2D.prototype.Contains = function(p)
{
	return p.x >= this.position.x - this.width / 2 && p.x <= this.position.x + this.width / 2 
		&& p.y <= this.position.y + this.height / 2 && p.y >= this.position.y - this.height / 2;
}

BoxCollider2D.prototype.Intersects = function(other)
{
	var topLeft = new vec2(this.position.x - this.width / 2, this.position.y + this.height / 2);
	var topRight = new vec2(this.position.x + this.width / 2, this.position.y + this.height / 2);
	var bottomLeft = new vec2(this.position.x - this.width / 2, this.position.y - this.height / 2);
	var bottomRight = new vec2(this.position.x + this.width / 2, this.position.y - this.height / 2);
	
	var otherTopLeft = new vec2(other.position.x - other.width / 2, other.position.y + other.height / 2);
	var otherTopRight = new vec2(other.position.x + other.width / 2, other.position.y + other.height / 2);
	var otherBottomLeft = new vec2(other.position.x - other.width / 2, other.position.y - other.height / 2);
	var otherBottomRight = new vec2(other.position.x + other.width / 2, other.position.y - other.height / 2);
	
	return this.Contains(otherTopLeft) || this.Contains(otherTopRight) || this.Contains(otherBottomLeft) || this.Contains(otherBottomRight)
		|| other.Contains(topLeft) || other.Contains(topRight) || other.Contains(bottomLeft) || other.Contains(bottomRight)
}

BoxCollider2D.prototype.Init = function()
{
	this.position.x = this.owner.transform.position.x;
	this.position.y = this.owner.transform.position.y;
}

BoxCollider2D.prototype.Update = function()
{
	this.position = new vec2(this.owner.transform.position.x, this.owner.transform.position.y);
}

BoxCollider2D.prototype.Render = function()
{
	
}