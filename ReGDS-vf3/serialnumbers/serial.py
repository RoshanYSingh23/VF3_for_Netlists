# File paths
input_file = "/home/natraj/ReGDS-vf2pp/serialnumbers/serials.txt"  # Replace with your input file name
output_file = "/home/natraj/ReGDS-vf2pp/serialnumbers/output.txt"  # Replace with your desired output file name

# Open the input file and create the output file
with open(input_file, "r") as infile, open(output_file, "w") as outfile:
    # Iterate through each line with a counter
    for count, line in enumerate(infile, start=1):
        # Split the line into words and update the first word
        parts = line.split()
        parts[0] = f"M{count}"  # Replace the original serial number
        # Write the updated line to the output file
        outfile.write(" ".join(parts) + "\n")

print(f"Updated file saved as {output_file}")
