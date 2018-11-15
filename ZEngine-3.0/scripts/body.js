//code.iamkate.com
function Queue(){var a=[],b=0;this.getLength=function(){return a.length-b};this.isEmpty=function(){return 0==a.length};this.enqueue=function(b){a.push(b)};this.dequeue=function(){if(0!=a.length){var c=a[b];2*++b>=a.length&&(a=a.slice(b),b=0);return c}};this.peek=function(){return 0<a.length?a[b]:void 0}};

function BodyComponent()
{
	this.child = null;
	this.past = new Queue();
	this.pool = new Queue();
}

BodyComponent.prototype.Init = function()
{
	
}

BodyComponent.prototype.Update = function()
{
	if (this.pool.getLength() == 0)
	{
		this.past.enqueue(this.owner.transform.position);
	}
	else
	{
		var pos = this.pool.dequeue();
		this.owner.transform.GetPosition(pos);
		
		this.past.enqueue(pos);
	}
	
	if (this.past.getLength() > 5)
	{
		var childPos = this.past.dequeue();
		
		if (this.child != null)
			this.child.transform.position = childPos;
		
		this.pool.enqueue(childPos);
	}
}

BodyComponent.prototype.Render = function()
{
	
}