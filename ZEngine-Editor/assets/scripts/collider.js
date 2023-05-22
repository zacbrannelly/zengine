function BoxCollider2D()
{
	this.position = new vec2(0, 0);
		
	this.width = 1;
	this.height = 1;
	
	// Cache onwer position so we don't constantly create new vec3's
	this.ownerPos = new vec3(0, 0, 0);
}

BoxCollider2D.prototype.Contains = function(p)
{
	return p.x >= this.position.x - this.width / 2 && p.x <= this.position.x + this.width / 2 
		&& p.y <= this.position.y + this.height / 2 && p.y >= this.position.y - this.height / 2;
}

BoxCollider2D.prototype.Intersects = function(other)
{
	return ((this.position.x - this.width / 2 < other.position.x + other.width / 2 && this.position.x - this.width / 2 > other.position.x - other.width / 2)
		|| (this.position.x + this.width / 2 > other.position.x - other.width / 2 && this.position.x + this.width / 2 < other.position.x + other.width / 2))
		&& ((this.position.y - this.height / 2 < other.position.y + other.height / 2 && this.position.y - this.height / 2 > other.position.y - other.height / 2)
		|| (this.position.y + this.height / 2 > other.position.y - other.height / 2 && this.position.y + this.height / 2 < other.position.y + other.height / 2));
		
}

BoxCollider2D.prototype.Init = function()
{
	this.position.x = this.owner.transform.position.x;
	this.position.y = this.owner.transform.position.y;
}

BoxCollider2D.prototype.Update = function()
{
	this.ownerPos = this.owner.transform.GetPosition(this.ownerPos);
	
	this.position.x = this.ownerPos.x;
	this.position.y = this.ownerPos.y;
}

BoxCollider2D.prototype.Render = function()
{
	
}