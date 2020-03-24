<?php

/**
 * Automatically includes classes
 * 
 * @throws Exception
 * 
 * @param  string $class_name  Name of the class to load
 * @return void
 */
function __autoload($class_name)
{
    // Customize this to your root Flourish directory
    $flourish_root = $_SERVER['DOCUMENT_ROOT'] . 'flourish/';
    
    $file = $flourish_root . $class_name . '.php';

    if (file_exists($file)) {
        include $file;
        return;
    }
    
    throw new Exception('The class ' . $class_name . ' could not be loaded');
}

$shortopts = "i:";

$options = getopt($shortopts);

$input_file = new fFile($options['i']);
$output = "";
$layer_height = "0.05";

foreach ($input_file as $line) {
	if (fUTF8::istr($line, 'layer_height')) {
		preg_match("/[-+]?[0-9]*\.?[0-9]+/", $line, $matches);
		$layer_height = $matches[0];
	}
	if (fUTF8::istr($line, 'G1')) {

		// This line is a G1, remove the E value ("E3.4567")
		if (fUTF8::istr($line, 'E')) {
			$line = preg_replace("/E\S+\s?/", "", $line);
		} else { // This line is a G0
			$line = preg_replace("/G1/", "G0", $line);
		}

		// This line has a Z move 
		if (fUTF8::istr($line, 'Z')) {			
			$output .= "G0 X-100 F3000\n"; // Move the laser to a safe position
			//$output .= "G4 P1000\n";  Pause for 2 sec, added by Daniel
			$output .= "T0 ;select E0 axis\n"; // Select E0, where positive movement is down
			$output .= "G0 E" . strval(floatval($layer_height) * 3) . " F200\n";
			$output .= "T1 ;select E1 axis\n"; // Select E1, where positive movement is up
			$output .= "G0 E" . strval(floatval($layer_height) * 6) . " F200\n"; // move by 3x layer height
			$output .= "G0 Z-244 F3000\n"; // Move the wiper 100mm
			$output .= "G0 Z0 F3000\n"; // Move the wiper back to 0
		} else {
			$output .= $line;
		}

	} else {
		if (fUTF8::istr($line, 'M104') || fUTF8::istr($line, 'M106') || fUTF8::istr($line, 'M109') || fUTF8::istr($line, 'M190') || fUTF8::istr($line, 'M140')) {
			// don't pass temperature lines through
		} else {
			$output .= $line; // this line does not start with G, let it pass through
		}
	}
}

$output_file = fFile::create('output.gcode', $output);

?>
