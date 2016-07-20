class Pie {
	static generatePie(centerX, centerY, startAngle, endAngle, radius) {
		//Calculate the first point location
		var p1x = ( Math.cos(startAngle *	(Math.PI / 180)) * radius ) + centerX;
		var p1y = ( Math.sin(startAngle *	(Math.PI / 180)) * radius ) + centerY;
		//Calculate the second point location
		var p2x = ( Math.cos(endAngle *	(Math.PI / 180)) * radius ) + centerX;
		var p2y = ( Math.sin(endAngle *	(Math.PI / 180)) * radius ) + centerY;

		//Definition
		var d = "";
		//Specify the origin
		d += "M" + centerX + "," + centerY;
		//Travel in a straight line to the first point
		d += "L" + p1x + "," + p1y;
		//Travel in an arc to the second point
		d += "A";
			//Set the x and y radius of the arc
			d += radius + "," + radius + ",";
			//Set the x-axis rotation
			d += "0,";
			//Set the large-arc flag, 1 = angle > 180, 0 = angle < 180
			d += (endAngle - startAngle > 180 ? "1," : "0,");
			//Set the sweep flag
			d += "1,"; //Not sure what this does
			//Set the ending coordinates
			d += p2x + "," + p2y;
		//Return to center
		d += "Z";

		//Create the element
		var path = document.createElementNS("http://www.w3.org/2000/svg", "path");
		//Set the properties
		path.setAttribute("d",d);
		path.setAttribute("stroke-width", "1");
		path.setAttribute("opacity", "1");
		path.setAttribute("style", "opacity: 1;");

		//Return path object
		return path;
	}
}