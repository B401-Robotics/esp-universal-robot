import { CirclePower } from 'lucide-react'
import { Button } from '@/components/ui/button'
import axios from 'axios'
import { useState, useEffect } from 'react'
import { useToast } from '@/components/ui/use-toast'

export default function Landing() {
	const { toast } = useToast()
	const [esp32Ip, setEsp32Ip] = useState('')
	const [relay, setRelay] = useState([0, 0, 0, 0, 0, 0, 0, 0])

	const fetchEsp32Ip = () => {
		axios
			.get('/ip') // Ini adalah endpoint yang harus diarahkan ke ESP32
			.then(response => {
				if (response.data.ip === undefined) throw new Error('IP not found')
				setEsp32Ip(response.data.ip) // Set alamat IP dinamis
				console.log('ESP32 IP:', response.data.ip)
			})
			.catch(error => {
				console.error('Error getting ESP32 IP:', error)
			})
	}

	const fetchRelayStatus = () => {
		axios
			.get('/status')
			.then(response => {
				// console.log('Response:', response.data)
				if (response.data.relay === undefined) throw new Error('Relay status not found')
				setRelay(response.data.relay)
				console.log('Relay:', response.data.relay)
			})
			.catch(error => {
				console.error('Error getting relay:', error)
			})
	}

	useEffect(() => {
		fetchEsp32Ip() // Mendapatkan alamat IP saat komponen dimuat
		fetchRelayStatus() // Mendapatkan status relay saat komponen dimuat
	}, [])

	// Endpoint API berdasarkan alamat IP ESP32
	const apiEndpoint = `http://${esp32Ip}/relay/`

	// Fungsi untuk mengirim permintaan POST
	const handleRelayControl = (relayNumber: number) => {
		const newState = relay[relayNumber] === 1 ? 'off' : 'on'

		axios({
			method: 'post',
			url: apiEndpoint + relayNumber,
			data: 'state=' + newState,
			headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
		})
			.then(response => {
				console.log('Relay', relayNumber, 'response:', response.data)
				setRelay(prevState => {
					const newState = [...prevState]
					newState[relayNumber] = newState[relayNumber] === 1 ? 0 : 1
					return newState
				})
				toast({
					title: 'Relay status updated',
					description: 'Relay' + response.data.relay + 'status updated',
				})
			})
			.catch(error => {
				console.error('Relay', relayNumber, 'error:', error)
				toast({
					variant: 'destructive',
					title: 'Error updating relay status',
				})
			})
	}

	return (
		<div className="relative">
			<h1 className="text-5xl font-medium 2xl:text-8xl">Universal Robot</h1>
			<p className="text-xl text-muted-foreground 2xl:text-2xl">Relay Controller</p>

			<div className="mt-8 flex h-fit flex-wrap items-center justify-center gap-5 text-sm">
				{relay.map((value, index) => (
					<Button
						className="w-1/2 sm:w-1/3"
						variant={value === 1 ? 'default' : 'secondary'}
						key={index}
						onClick={() => handleRelayControl(index)}>
						<CirclePower className="mr-2 h-4 w-4" /> Relay {index + 1} ({value ? 'ON' : 'OFF'})
					</Button>
				))}
			</div>
		</div>
	)
}
