var docRef = app.activeDocument;
var docPath;

try{
	docPath = docRef.path;
	}
catch(ex){
	docPath = Folder.myDocuments;
}

var ui = new Window("dialog", "Save Image as Squares");

var grpTxt = ui.add("group");
grpTxt.add("statictext", undefined, "Destination:");
var txtPath = grpTxt.add("edittext", [0, 0, 240, 20], docPath);

var grpBtns = ui.add ("group");
//grpBtns.alignment = "bottom";
grpBtns.add ("button", undefined, "OK");
grpBtns.add ("button", undefined, "Cancel");

if(ui.show() == 1){
	main(txtPath.text, "PNG");
}else{
}

function main(path, fileType){
	// Rollback state
	var histStateBegin = docRef.activeHistoryState;

	// Save the current preferences
	var startRulerUnits = app.preferences.rulerUnits
	var startDisplayDialogs = app.displayDialogs

	app.preferences.rulerUnits = Units.PIXELS;
	app.preferences.displayDialogs = DialogModes.NO;

	alert("docRef: " + docRef);

	// resize image height to power of 2
	docRef.resizeImage(undefined, nearestPowerOf2(docRef.height));

	var h = docRef.height;
	var w = docRef.width;

	//alert("w: " + w + "px h: " + h + "px");
	var ratio = w / h;
	//alert(w + "/" + h + ": " + ratio);
	var numSections = Math.ceil(ratio);
	//alert("numSections: " + numSections);

	// Progress Bar
	var w = new Window ('palette', "Saving...");
	w.pbar = w.add ('progressbar', undefined, 0, numSections);
	w.pbar.preferredSize.width = 300;
	w.lblMessage = w.add("staticText", [0, 0, 300, 20], "Please wait...");
	w.layout.layout();
	w.show();

	var histStateExport = docRef.activeHistoryState;

	try{
		for (var i = 0; i < numSections; ++i) {
			var filePath = path + "/" + docRef.name + "_" + (i + 1) + ".png";

			w.lblMessage.text = "Exporting " + (i+1) + " of " + numSections + "...";

			// Crop region (left, top, right, bottom)
			docRef.crop([i * h, 0, i*h + h, h]);
    
			if (docRef.width < h) {
				// Resize Canvas
				docRef.resizeCanvas(h, h, AnchorPosition.MIDDLELEFT);
			}

			// Save image
			
			saveImage(filePath);

			// Undo
			docRef.activeHistoryState = histStateExport;
			
			w.pbar.value = i+1;
			
		}

		alert("finished!");

	}catch(ex){
		alert(ex);
		
	}finally{
		docRef.activeHistoryState = histStateBegin;

		w.close();

		// Restore old preferences
		var startRulerUnits = app.preferences.rulerUnits;
		var startDisplayDialogs = app.displayDialogs;
	}

	
}

function nearestPowerOf2(n){
	n--;
	n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32
	n |= n >> 2;   // and then or the results.
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;  

	return n;
}


function saveImage(filePath, fileType) {
    var file = new File(filePath);
    var opts;
	
	switch(fileType){
		case "PNG":
		default:
			opts = new PNGSaveOptions();
			break;
	}
    
    docRef.saveAs(file, opts, true, Extension.LOWERCASE);
}
