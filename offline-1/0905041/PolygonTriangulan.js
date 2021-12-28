//polygon triangulation implementation in javascript

function ReadVertices()
{
    var vertices = [];
    var n = parseInt(prompt("Enter number of vertices"));
    for(var i=0;i<n;i++)
    {
    var x = parseFloat(prompt("Enter x coordinate of vertex "+(i+1)));
    var y = parseFloat(prompt("Enter y coordinate of vertex "+(i+1)));
    vertices[i] = new Vertex(x,y);
    }
}

function Vertex(x,y)
{
    this.x = x;
    this.y = y;
}

function Polygon(vertices)
{
    this.vertices = vertices;
}

function Triangulate(polygon)
{
    var triangles = [];
    var n = polygon.vertices.length;
    for(var i=0;i<n;i++)
    {
        var v1 = polygon.vertices[i];
        var v2 = polygon.vertices[(i+1)%n];
        var v3 = polygon.vertices[(i+2)%n];
        triangles[i] = new Triangle(v1,v2,v3);
    }
    return triangles;
}

function Triangle(v1,v2,v3)
{
    this.v1 = v1;
    this.v2 = v2;
    this.v3 = v3;
}

function PrintVertices()
{
    var n = vertices.length;
    for(var i=0;i<n;i++)
    {
        var v = vertices[i];
        console.log("Vertex "+(i+1)+" : ("+v.x+","+v.y+")");
    }
}

function main()
{
    var vertices = [];
    vertices[0] = new Vertex(0,0);
    vertices[1] = new Vertex(1,0);
    vertices[2] = new Vertex(1,1);
    vertices[3] = new Vertex(0,1);
    var polygon = new Polygon(vertices);
    var triangles = Triangulate(polygon);
    for(var i=0;i<triangles.length;i++)
    {
        var triangle = triangles[i];
        var v1 = triangle.v1;
        var v2 = triangle.v2;
        var v3 = triangle.v3;
        console.log("Triangle "+(i+1)+" : "+v1.x+" "+v1.y+" "+v2.x+" "+v2.y+" "+v3.x+" "+v3.y);
    }
}


function EarInit(){
    var vertices = [];
    vertices[0] = new Vertex(0,0);
    vertices[1] = new Vertex(1,0);
    vertices[2] = new Vertex(1,1);
    vertices[3] = new Vertex(0,1);
    var polygon = new Polygon(vertices);
    var triangles = Triangulate(polygon);
    for(var i=0;i<triangles.length;i++){
        var triangle = triangles[i];
        var v1 = triangle.v1;
        var v2 = triangle.v2;
        var v3 = triangle.v3;
        console.log("Triangle "+(i+1)+" : "+v1.x+" "+v1.y+" "+v2.x+" "+v2.y+" "+v3.x+" "+v3.y);
    }




    


}


function triangulation()
{
    ReadVertices();
    PrintVertices();
    Triangulate();
}


EarInit();


